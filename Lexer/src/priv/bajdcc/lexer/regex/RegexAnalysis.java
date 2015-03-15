package priv.bajdcc.lexer.regex;

import java.util.HashMap;
import java.util.Stack;

import priv.bajdcc.lexer.regex.Charset.CharacterType;
import priv.bajdcc.lexer.token.TokenUtility;
import priv.bajdcc.lexer.token.TokenUtility.MetaType;
import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;

/**
 * ## 正则表达式分析工具 ##<br/>
 * 用于生成语法树<br/>
 * 语法同一般的正则表达式，有贪婪模式，没有前/后向匹配， 没有捕获功能，仅用于匹配。
 * 
 * @author bajdcc
 */
public class RegexAnalysis {
	/**
	 * 存储正则表达式
	 */
	private String m_strPattern;

	/**
	 * 当前的分析信息
	 */
	private RegexAnalysisData m_Data = new RegexAnalysisData();

	/**
	 * 表达式树根结点
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
	 * ## 编译表达式 ##<br/>
	 * 表达式 => AST => ENFA => NFA => DFA => MFA
	 * 
	 * @throws RegexException
	 */
	private void compile() throws RegexException {
		translate();
		m_Expression = analysis(MetaType.END.getChar(), MetaType.END);
	}

	private IRegexComponent analysis(char terminal, MetaType meta)
			throws RegexException {
		Constructure sequence = new Constructure(false);// 建立序列以存储表达式
		Constructure branch = null;// 建立分支以存储'|'型表达式，是否是分支有待预测
		Constructure result = sequence;

		for (;;) {
			if ((m_Data.m_kMeta == meta && m_Data.m_chCurrent == terminal)) {// 结束字符
				if (m_Data.m_iIndex == 0) {// 表达式为空
					err(RegexError.NULL);
				} else if (sequence.m_arrComponents.isEmpty()) {// 部件为空
					err(RegexError.INCOMPLETE);
				} else {
					next();
					break;// 正常终止
				}
			} else if (m_Data.m_kMeta == MetaType.END) {
				err(RegexError.INCOMPLETE);
			}
			IRegexComponent expression = null;// 当前待赋值的表达式
			switch (m_Data.m_kMeta) {
			case BAR:// '|'
				next();
				if (sequence.m_arrComponents.isEmpty())// 在此之前没有存储表达式 (|...)
				{
					err(RegexError.INCOMPLETE);
				} else {
					if (branch == null) {// 分支为空，则建立分支
						branch = new Constructure(true);
						branch.m_arrComponents.add(sequence);// 用新建的分支包含并替代当前序列
						result = branch;
					}
					sequence = new Constructure(false);// 新建一个序列
					branch.m_arrComponents.add(sequence);
				}
				break;
			case LPARAN:// '('
				next();
				expression = analysis(MetaType.RPARAN.getChar(),
						MetaType.RPARAN);// 递归分析
				break;
			default:
				break;
			}

			if (expression == null) {// 当前不是表达式，则作为字符
				Charset charset = new Charset();// 当前待分析的字符集
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
					escape(charset, true);// 处理转义
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

			Repetition rep = null;// 循环
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
				rep.m_bGreedy = false;// 非贪婪模式
			}
		}

		return result;
	}

	/**
	 * 处理转义字符
	 * 
	 * @param charset
	 *            字符集
	 * @param extend
	 *            是否支持扩展如\d \w等
	 * @throws RegexException
	 */
	private void escape(Charset charset, boolean extend) throws RegexException {
		char ch = m_Data.m_chCurrent;
		if (m_Data.m_kMeta == MetaType.CHARACTER) {// 字符
			next();
			if (TokenUtility.isLetter(ch)) {// 如果为字母
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
					charset.m_bReverse = true;// 大写则取反
				}
				if (!charset.addChar(ch)){
					err(RegexError.RANGE);
				}
			} else {
				if (extend) {
					char cl = Character.toLowerCase(ch);
					switch (cl) {
					case 'd':// 数字
						charset.addRange('0', '9');
						break;
					case 'a':// 字母
						charset.addRange('a', 'z');
						charset.addRange('A', 'Z');
						break;
					case 'w':// 标识符
						charset.addRange('a', 'z');
						charset.addRange('A', 'Z');
						charset.addRange('0', '9');
						charset.addChar('_');
						break;
					case 's':// 空白字符
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
		} else {// 功能字符则转义
			next();
			if (!charset.addChar(ch)){
				err(RegexError.RANGE);
			}
		}
	}

	/**
	 * 处理字符集合
	 * 
	 * @param charset
	 *            字符集
	 * @throws RegexException
	 */
	private void range(Charset charset) throws RegexException {
		if (m_Data.m_kMeta == MetaType.CARET) {// '^'取反
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
	 * 处理字符
	 * 
	 * @param charset
	 *            字符集
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
	 * 处理量词
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
	 * 数字进制转换（分八进制OCT \o，两位十六进制HEX \x，四位十六进制UNICODE \\u）
	 * 
	 * @param base
	 *            进制
	 * @param count
	 *            长度
	 * @return 数字
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
	 * 十进制数字转换
	 * 
	 * @return 数字
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
	 * 抛出错误
	 * 
	 * @param error
	 *            错误类型
	 * @throws RegexException
	 */
	private void err(RegexError error) throws RegexException {
		throw new RegexException(error, m_Data.m_iIndex);
	}

	/**
	 * 处理下一个字符
	 */
	private void next() {
		if (available()) {
			advance();
		}
		translate();
	}

	/**
	 * 翻译当前字符
	 */
	private void translate() {
		if (!available()) {
			m_Data.m_chCurrent = 0;
			m_Data.m_kMeta = MetaType.END;
			return;
		}
		m_Data.m_chCurrent = current();
		if (g_mapMeta.containsKey(m_Data.m_chCurrent)) {
			m_Data.m_kMeta = g_mapMeta.get(m_Data.m_chCurrent);// 功能字符
		} else {
			m_Data.m_kMeta = MetaType.CHARACTER;// 一般字符
		}
	}

	/**
	 * 判断当前位置不是末尾
	 * 
	 * @return 当前字符是否有效
	 */
	private boolean available() {
		return m_Data.m_iIndex < m_strPattern.length();
	}

	/**
	 * 前进一个字符（look forward）
	 * 
	 */
	private void advance() {
		m_Data.m_iIndex++;
	}

	/**
	 * 后退一个字符（look back）
	 * 
	 */
	private void rollback() {
		m_Data.m_iIndex--;
	}

	/**
	 * 获得当前字符
	 * 
	 * @return 当前字符
	 */
	private char current() {
		return m_strPattern.charAt(m_Data.m_iIndex);
	}

	/**
	 * 确认当前字符
	 * 
	 * @param meta
	 *            类型
	 * @param error
	 *            抛出的错误
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
		RegexToString alg = new RegexToString();// 表达式树序列化算法初始化
		m_Expression.visit(alg);// 遍历树
		return alg.toString();
	}
}

/**
 * 分析时使用的堆栈数据
 */
class RegexAnalysisData {
	/**
	 * 当前处理的位置
	 */
	public int m_iIndex = 0;

	/**
	 * 字符
	 */
	public char m_chCurrent = 0;

	/**
	 * 字符类型
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