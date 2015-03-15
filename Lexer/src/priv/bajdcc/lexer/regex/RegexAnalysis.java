package priv.bajdcc.lexer.regex;

import java.util.HashMap;
import java.util.Stack;

import priv.bajdcc.lexer.regex.Charset.CharacterType;
import priv.bajdcc.lexer.token.TokenUtility;
import priv.bajdcc.lexer.token.TokenUtility.MetaType;
import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;

/**
 * ## ������ʽ�������� ##<br/>
 * ���������﷨��<br/>
 * �﷨ͬһ���������ʽ����̰��ģʽ��û��ǰ/����ƥ�䣬 û�в����ܣ�������ƥ�䡣
 * 
 * @author bajdcc
 */
public class RegexAnalysis {
	/**
	 * �洢������ʽ
	 */
	private String m_strPattern;

	/**
	 * ��ǰ�ķ�����Ϣ
	 */
	private RegexAnalysisData m_Data = new RegexAnalysisData();

	/**
	 * ���ʽ�������
	 */
	private IRegexComponent m_Expression = null;

	private static HashMap<Character, TokenUtility.MetaType> g_mapMeta = new HashMap<Character, TokenUtility.MetaType>();

	static {
		for (TokenUtility.MetaType meta : TokenUtility.MetaType.values()) {
			if (meta.getChar() != 0) {
				g_mapMeta.put(meta.getChar(), meta);
			}
		}
	}

	public RegexAnalysis(String pattern) throws RegexException {
		m_strPattern = pattern;
		compile();
	}

	/**
	 * ## ������ʽ ##<br/>
	 * ���ʽ => AST => ENFA => NFA => DFA => MFA
	 * 
	 * @throws RegexException
	 */
	private void compile() throws RegexException {
		translate();
		m_Expression = analysis(MetaType.END.getChar(), MetaType.END);
	}

	private IRegexComponent analysis(char terminal, MetaType meta)
			throws RegexException {
		Constructure sequence = new Constructure(false);// ���������Դ洢���ʽ
		Constructure branch = null;// ������֧�Դ洢'|'�ͱ��ʽ���Ƿ��Ƿ�֧�д�Ԥ��
		Constructure result = sequence;

		for (;;) {
			if ((m_Data.m_kMeta == meta && m_Data.m_chCurrent == terminal)) {// �����ַ�
				if (m_Data.m_iIndex == 0) {// ���ʽΪ��
					err(RegexError.NULL);
				} else if (sequence.m_arrComponents.isEmpty()) {// ����Ϊ��
					err(RegexError.INCOMPLETE);
				} else {
					next();
					break;// ������ֹ
				}
			} else if (m_Data.m_kMeta == MetaType.END) {
				err(RegexError.INCOMPLETE);
			}
			IRegexComponent expression = null;// ��ǰ����ֵ�ı��ʽ
			switch (m_Data.m_kMeta) {
			case BAR:// '|'
				next();
				if (sequence.m_arrComponents.isEmpty())// �ڴ�֮ǰû�д洢���ʽ (|...)
				{
					err(RegexError.INCOMPLETE);
				} else {
					if (branch == null) {// ��֧Ϊ�գ�������֧
						branch = new Constructure(true);
						branch.m_arrComponents.add(sequence);// ���½��ķ�֧�����������ǰ����
						result = branch;
					}
					sequence = new Constructure(false);// �½�һ������
					branch.m_arrComponents.add(sequence);
				}
				break;
			case LPARAN:// '('
				next();
				expression = analysis(MetaType.RPARAN.getChar(),
						MetaType.RPARAN);// �ݹ����
				break;
			default:
				break;
			}

			if (expression == null) {// ��ǰ���Ǳ��ʽ������Ϊ�ַ�
				Charset charset = new Charset();// ��ǰ���������ַ���
				expression = charset;
				switch (m_Data.m_kMeta) {
				case CARET:// '^'
					charset.m_kChar = CharacterType.BEGIN;
					break;
				case DOLLAR:// '$'
					charset.m_kChar = CharacterType.END;
					break;
				case ESCAPE:// '\\'
					next();
					escape(charset, true);// ����ת��
					break;
				case LSQUARE: // '['
					next();
					range(charset);
					break;
				case END: // '\0'
					return result;
				default:
					if (!charset.addChar(m_Data.m_chCurrent)){
						err(RegexError.RANGE);
					}
					next();
					break;
				}
			}

			Repetition rep = null;// ѭ��
			switch (m_Data.m_kMeta) {
			case QUERY:// '?'
				next();
				rep = new Repetition(expression, 0, 1, false);
				sequence.m_arrComponents.add(rep);
				break;
			case PLUS:// '+'
				next();
				rep = new Repetition(expression, 1, -1, false);
				sequence.m_arrComponents.add(rep);
				break;
			case STAR:// '*'
				next();
				rep = new Repetition(expression, 0, -1, false);
				sequence.m_arrComponents.add(rep);
				break;
			case LBRACE: // '{'
				next();
				rep = new Repetition(expression, 0, -1, false);
				quantity(rep);
				sequence.m_arrComponents.add(rep);
				break;
			default:
				sequence.m_arrComponents.add(expression);
				break;
			}

			if (m_Data.m_kMeta == MetaType.QUERY) {// '?'
				next();
				rep.m_bGreedy = false;// ��̰��ģʽ
			}
		}

		return result;
	}

	/**
	 * ����ת���ַ�
	 * 
	 * @param charset
	 *            �ַ���
	 * @param extend
	 *            �Ƿ�֧����չ��\d \w��
	 * @throws RegexException
	 */
	private void escape(Charset charset, boolean extend) throws RegexException {
		char ch = m_Data.m_chCurrent;
		if (m_Data.m_kMeta == MetaType.CHARACTER) {// �ַ�
			next();
			if (TokenUtility.isLetter(ch)) {// ���Ϊ��ĸ
				if (ch == 'r') {
					ch = '\r';
				} else if (ch == 'n') {
					ch = '\n';
				} else if (ch == 't') {
					ch = '\t';
				} else if (ch == 'b') {
					ch = '\b';
				} else if (ch == 'f') {
					ch = '\f';
				} else if (ch == 'x') {
					int d = digit(16, 2);
					if (d == -1) {
						err(RegexError.ESCAPE);
					}
					ch = (char) d;
				} else if (ch == 'o') {
					int d = digit(8, 3);
					if (d == -1) {
						err(RegexError.ESCAPE);
					}
					ch = (char) d;
				} else if (ch == 'u') {
					int d = digit(16, 4);
					if (d == -1) {
						err(RegexError.ESCAPE);
					}
					ch = (char) d;
				} else if (TokenUtility.isUpperLetter(ch) || ch == '.') {
					charset.m_bReverse = true;// ��д��ȡ��
				}
				if (!charset.addChar(ch)){
					err(RegexError.RANGE);
				}
			} else {
				if (extend) {
					char cl = Character.toLowerCase(ch);
					switch (cl) {
					case 'd':// ����
						charset.addRange('0', '9');
						break;
					case 'a':// ��ĸ
						charset.addRange('a', 'z');
						charset.addRange('A', 'Z');
						break;
					case 'w':// ��ʶ��
						charset.addRange('a', 'z');
						charset.addRange('A', 'Z');
						charset.addRange('0', '9');
						charset.addChar('_');
						break;
					case 's':// �հ��ַ�
						charset.addChar('\r');
						charset.addChar('\n');
						charset.addChar('\t');
						charset.addChar('\b');
						charset.addChar('\f');
						charset.addChar(' ');
						break;
					default:
						err(RegexError.ESCAPE);
						break;
					}
				}
			}
		} else if (m_Data.m_kMeta == MetaType.END) {
			err(RegexError.INCOMPLETE);
		} else {// �����ַ���ת��
			next();
			if (!charset.addChar(ch)){
				err(RegexError.RANGE);
			}
		}
	}

	/**
	 * �����ַ�����
	 * 
	 * @param charset
	 *            �ַ���
	 * @throws RegexException
	 */
	private void range(Charset charset) throws RegexException {
		if (m_Data.m_kMeta == MetaType.CARET) {// '^'ȡ��
			next();
			charset.m_bReverse = true;
		}
		while (m_Data.m_kMeta != MetaType.RSQUARE) {// ']'
			if (m_Data.m_kMeta == MetaType.CHARACTER) {
				character(charset);
				char lower = m_Data.m_chCurrent; // lower bound
				next();
				if (m_Data.m_kMeta == MetaType.DASH) {// '-'
					next();
					character(charset);
					char upper = m_Data.m_chCurrent; // upper bound
					next();
					if (lower > upper) {// check bound
						err(RegexError.RANGE);
					}
					if (!charset.addRange(lower, upper)) {
						err(RegexError.RANGE);
					}
				} else {
					if (!charset.addChar(lower)){
						err(RegexError.RANGE);
					}
				}
			} else if (m_Data.m_kMeta == MetaType.ESCAPE) {
				next();
				escape(charset, false);
			} else {
				err(RegexError.RANGE);
			}
		}
		next();
	}

	/**
	 * �����ַ�
	 * 
	 * @param charset
	 *            �ַ���
	 * @throws RegexException
	 */
	private void character(Charset charset) throws RegexException {
		if (m_Data.m_kMeta == MetaType.ESCAPE) {// '\\'
			next();
			escape(charset, false);
		} else if (m_Data.m_kMeta == MetaType.END) {// '\0'
			err(RegexError.INCOMPLETE);
		} else if (m_Data.m_kMeta != MetaType.CHARACTER
				&& m_Data.m_kMeta != MetaType.DASH) {
			err(RegexError.CTYPE);
		}
	}

	/**
	 * ��������
	 * 
	 * @throws RegexException
	 */
	private void quantity(Repetition rep) throws RegexException {
		int lower = 0, upper = -1;
		lower = digit();
		if (lower == -1) {
			err(RegexError.BRACE);
		}
		if (m_Data.m_kMeta == MetaType.COMMA) {
			next();
			upper = digit();
			if (upper == -1) {
				err(RegexError.BRACE);
			}
		}
		if (upper < lower) {
			err(RegexError.RANGE);
		}
		expect(MetaType.RBRACE, RegexError.BRACE);
		rep.m_iLowerBound = lower;
		rep.m_iUpperBound = upper;
	}

	/**
	 * ���ֽ���ת�����ְ˽���OCT \o����λʮ������HEX \x����λʮ������UNICODE \\u��
	 * 
	 * @param base
	 *            ����
	 * @param count
	 *            ����
	 * @return ����
	 */
	private int digit(int base, int count) {
		int chv, val = 0;
		try {
			while (count != 0) {
				chv = Integer.valueOf(m_Data.m_chCurrent + "", base);
				--count;
				val *= base;
				val += chv;
				next();
			}
		} catch (NumberFormatException e) {
			val = -1;
		}
		return val;
	}

	/**
	 * ʮ��������ת��
	 * 
	 * @return ����
	 */
	private int digit() {
		int index = m_Data.m_iIndex;
		while (Character.isDigit(m_Data.m_chCurrent)) {
			next();
		}
		try {
			return Integer.valueOf(
					m_strPattern.substring(index, m_Data.m_iIndex), 10);
		} catch (NumberFormatException e) {
			return -1;
		}
	}

	/**
	 * �׳�����
	 * 
	 * @param error
	 *            ��������
	 * @throws RegexException
	 */
	private void err(RegexError error) throws RegexException {
		throw new RegexException(error, m_Data.m_iIndex);
	}

	/**
	 * ������һ���ַ�
	 */
	private void next() {
		if (available()) {
			advance();
		}
		translate();
	}

	/**
	 * ���뵱ǰ�ַ�
	 */
	private void translate() {
		if (!available()) {
			m_Data.m_chCurrent = 0;
			m_Data.m_kMeta = MetaType.END;
			return;
		}
		m_Data.m_chCurrent = current();
		if (g_mapMeta.containsKey(m_Data.m_chCurrent)) {
			m_Data.m_kMeta = g_mapMeta.get(m_Data.m_chCurrent);// �����ַ�
		} else {
			m_Data.m_kMeta = MetaType.CHARACTER;// һ���ַ�
		}
	}

	/**
	 * �жϵ�ǰλ�ò���ĩβ
	 * 
	 * @return ��ǰ�ַ��Ƿ���Ч
	 */
	private boolean available() {
		return m_Data.m_iIndex < m_strPattern.length();
	}

	/**
	 * ǰ��һ���ַ���look forward��
	 * 
	 */
	private void advance() {
		m_Data.m_iIndex++;
	}

	/**
	 * ����һ���ַ���look back��
	 * 
	 */
	private void rollback() {
		m_Data.m_iIndex--;
	}

	/**
	 * ��õ�ǰ�ַ�
	 * 
	 * @return ��ǰ�ַ�
	 */
	private char current() {
		return m_strPattern.charAt(m_Data.m_iIndex);
	}

	/**
	 * ȷ�ϵ�ǰ�ַ�
	 * 
	 * @param meta
	 *            ����
	 * @param error
	 *            �׳��Ĵ���
	 * @throws RegexException
	 */
	private void expect(MetaType meta, RegexError error) throws RegexException {
		if (m_Data.m_kMeta != meta) {
			next();
		} else {
			err(error);
		}
	}

	@Override
	public String toString() {
		RegexToString alg = new RegexToString();// ���ʽ�����л��㷨��ʼ��
		m_Expression.visit(alg);// ������
		return alg.toString();
	}
}

/**
 * ����ʱʹ�õĶ�ջ����
 */
class RegexAnalysisData {
	/**
	 * ��ǰ�����λ��
	 */
	public int m_iIndex = 0;

	/**
	 * �ַ�
	 */
	public char m_chCurrent = 0;

	/**
	 * �ַ�����
	 */
	public MetaType m_kMeta = MetaType.END;

	public RegexAnalysisData() {

	}

	public RegexAnalysisData(int index, char current, MetaType meta) {
		m_iIndex = index;
		m_chCurrent = current;
		m_kMeta = meta;
	}
}