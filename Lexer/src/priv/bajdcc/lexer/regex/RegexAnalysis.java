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
	private String strPattern;

	/**
	 * ��ǰ�ķ�����Ϣ
	 */
	private RegexAnalysisData Data = new RegexAnalysisData();

	/**
	 * ���ʽ�������
	 */
	private IRegexComponent Expression = null;

	private static HashMap<Character, TokenUtility.MetaType> mapMeta = new HashMap<Character, TokenUtility.MetaType>();

	static {
		for (TokenUtility.MetaType meta : TokenUtility.MetaType.values()) {
			if (meta.getChar() != 0) {
				mapMeta.put(meta.getChar(), meta);
			}
		}
	}

	public RegexAnalysis(String pattern) throws RegexException {
		strPattern = pattern;
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
		Expression = analysis(MetaType.END.getChar(), MetaType.END);
	}

	private IRegexComponent analysis(char terminal, MetaType meta)
			throws RegexException {
		Constructure sequence = new Constructure(false);// ���������Դ洢���ʽ
		Constructure branch = null;// ������֧�Դ洢'|'�ͱ��ʽ���Ƿ��Ƿ�֧�д�Ԥ��
		IRegexComponent result = sequence;

		for (;;) {
			if ((Data.kMeta == meta && Data.chCurrent == terminal)) {// �����ַ�
				if (Data.iIndex == 0) {// ���ʽΪ��
					err(RegexError.NULL);
				} else if (sequence.Components.isEmpty()) {// ����Ϊ��
					err(RegexError.INCOMPLETE);
				} else {
					break;// ������ֹ
				}
			}
			if (Data.kMeta == MetaType.END) {
				err(RegexError.INCOMPLETE);
			}
			IRegexComponent expression = null;// ��ǰ����ֵ�ı��ʽ
			switch (Data.kMeta) {
			case BAR:// '|'
				if (sequence.Components.isEmpty())// �ڴ�֮ǰû�д洢���ʽ (|...)
				{
					err(RegexError.INCOMPLETE);
				} else {
					if (branch == null) {// ��֧Ϊ�գ�������֧
						branch = new Constructure(true);
						branch.Components.add(sequence);// ���½��ķ�֧�����������ǰ����
						result = branch;
					}
					sequence = new Constructure(false);// �½�һ������
					branch.Components.add(sequence);
				}
				break;
			case LPARAN:// '('
				next();
				expression = analysis(MetaType.RPARAN.getChar(),
						MetaType.CHARACTER);// �ݹ����
				break;
			}

			Charset charset = new Charset();// ��ǰ���������ַ���
			switch (Data.kMeta) {
			case CARET:// '^'
				charset.kChar = CharacterType.BEGIN;
				break;
			case DOLLAR:// '$'
				charset.kChar = CharacterType.END;
				break;
			case ESCAPE:// '\\'
				next();
				escape(charset);// ����ת��
				break;
			}
		}

		return Expression;
	}

	/**
	 * ����ת���ַ�
	 * 
	 * @param charset
	 *            �ַ���
	 * @throws RegexException
	 */
	private void escape(Charset charset) throws RegexException {
		char ch = Data.chCurrent;
		if (Data.kMeta == MetaType.CHARACTER) {// �ַ�
			if (TokenUtility.isLetter(ch)) {// ���Ϊ��ĸ
				if (ch == 'r') {
					Data.chCurrent = '\r';
				} else if (ch == 'n') {
					Data.chCurrent = '\n';
				} else if (ch == 't') {
					Data.chCurrent = '\t';
				} else if (ch == 'b') {
					Data.chCurrent = '\b';
				} else if (ch == 'f') {
					Data.chCurrent = '\f';
				} else if (ch == 'x') {
					Data.chCurrent = (char)digit(16, 2);
				} else if (ch == 'o') {
					Data.chCurrent = (char)digit(8, 3);
				} else if (ch == 'u') {
					Data.chCurrent = (char)digit(16, 4);
				} else if (TokenUtility.isUpperLetter(ch) || ch == '.') {
					charset.bReverse = true;// ��д��ȡ��
				}
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
				}
			}
		} else if (Data.kMeta == MetaType.END) {
			err(RegexError.INCOMPLETE);
		} else {// �����ַ���ת��
			Data.kMeta = MetaType.CHARACTER;
			charset.addChar(ch);
		}
		err(RegexError.ESCAPE);
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
				chv = Integer.valueOf(Data.chCurrent + "", base);
				--count;
				val *= base;
				val += chv;
				next();
			}
		} catch (NumberFormatException e) {
			val = 0;
		}
		return val;
	}

	/**
	 * �׳�����
	 * 
	 * @param error
	 *            ��������
	 * @throws RegexException
	 */
	private void err(RegexError error) throws RegexException {
		throw new RegexException(error, Data.iIndex);
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
			Data.chCurrent = 0;
			Data.kMeta = MetaType.END;
			return;
		}
		Data.chCurrent = current();
		if (mapMeta.containsKey(Data.chCurrent)) {
			Data.kMeta = mapMeta.get(Data.chCurrent);// �����ַ�
		} else {
			Data.kMeta = MetaType.CHARACTER;// һ���ַ�
		}
	}

	/**
	 * �жϵ�ǰλ�ò���ĩβ
	 * 
	 * @return ��ǰ�ַ��Ƿ���Ч
	 */
	private boolean available() {
		return Data.iIndex < strPattern.length();
	}

	/**
	 * ǰ��һ���ַ���look forward��
	 * 
	 */
	private void advance() {
		Data.iIndex++;
	}

	/**
	 * ����һ���ַ���look back��
	 * 
	 */
	private void roolback() {
		Data.iIndex--;
	}

	/**
	 * ��õ�ǰ�ַ�
	 * 
	 * @return ��ǰ�ַ�
	 */
	private char current() {
		return strPattern.charAt(Data.iIndex);
	}

	private void expect(MetaType meta, RegexError error) throws RegexException {
		if (Data.kMeta != meta) {
			next();
		} else {
			err(error);
		}
	}
}

/**
 * ����ʱʹ�õĶ�ջ����
 */
class RegexAnalysisData {
	/**
	 * ��ǰ�����λ��
	 */
	public int iIndex = 0;

	/**
	 * �ַ�
	 */
	public char chCurrent = 0;

	/**
	 * �ַ�����
	 */
	public MetaType kMeta = MetaType.END;

	public RegexAnalysisData() {

	}

	public RegexAnalysisData(int index, char current, MetaType meta) {
		iIndex = index;
		chCurrent = current;
		kMeta = meta;
	}
}