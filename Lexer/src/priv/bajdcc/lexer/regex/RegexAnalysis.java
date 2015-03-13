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
	private String strPattern;

	/**
	 * 当前的分析信息
	 */
	private RegexAnalysisData Data = new RegexAnalysisData();

	/**
	 * 表达式树根结点
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
	 * ## 编译表达式 ##<br/>
	 * 表达式 => AST => ENFA => NFA => DFA => MFA
	 * 
	 * @throws RegexException
	 */
	private void compile() throws RegexException {
		translate();
		Expression = analysis(MetaType.END.getChar(), MetaType.END);
	}

	private IRegexComponent analysis(char terminal, MetaType meta)
			throws RegexException {
		Constructure sequence = new Constructure(false);// 建立序列以存储表达式
		Constructure branch = null;// 建立分支以存储'|'型表达式，是否是分支有待预测
		IRegexComponent result = sequence;

		for (;;) {
			if ((Data.kMeta == meta && Data.chCurrent == terminal)) {// 结束字符
				if (Data.iIndex == 0) {// 表达式为空
					err(RegexError.NULL);
				} else if (sequence.Components.isEmpty()) {// 部件为空
					err(RegexError.INCOMPLETE);
				} else {
					break;// 正常终止
				}
			}
			if (Data.kMeta == MetaType.END) {
				err(RegexError.INCOMPLETE);
			}
			IRegexComponent expression = null;// 当前待赋值的表达式
			switch (Data.kMeta) {
			case BAR:// '|'
				if (sequence.Components.isEmpty())// 在此之前没有存储表达式 (|...)
				{
					err(RegexError.INCOMPLETE);
				} else {
					if (branch == null) {// 分支为空，则建立分支
						branch = new Constructure(true);
						branch.Components.add(sequence);// 用新建的分支包含并替代当前序列
						result = branch;
					}
					sequence = new Constructure(false);// 新建一个序列
					branch.Components.add(sequence);
				}
				break;
			case LPARAN:// '('
				next();
				expression = analysis(MetaType.RPARAN.getChar(),
						MetaType.CHARACTER);// 递归分析
				break;
			}

			Charset charset = new Charset();// 当前待分析的字符集
			switch (Data.kMeta) {
			case CARET:// '^'
				charset.kChar = CharacterType.BEGIN;
				break;
			case DOLLAR:// '$'
				charset.kChar = CharacterType.END;
				break;
			case ESCAPE:// '\\'
				next();
				escape(charset);// 处理转义
				break;
			}
		}

		return Expression;
	}

	/**
	 * 处理转义字符
	 * 
	 * @param charset
	 *            字符集
	 * @throws RegexException
	 */
	private void escape(Charset charset) throws RegexException {
		char ch = Data.chCurrent;
		if (Data.kMeta == MetaType.CHARACTER) {// 字符
			if (TokenUtility.isLetter(ch)) {// 如果为字母
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
					charset.bReverse = true;// 大写则取反
				}
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
				}
			}
		} else if (Data.kMeta == MetaType.END) {
			err(RegexError.INCOMPLETE);
		} else {// 功能字符则转义
			Data.kMeta = MetaType.CHARACTER;
			charset.addChar(ch);
		}
		err(RegexError.ESCAPE);
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
	 * 抛出错误
	 * 
	 * @param error
	 *            错误类型
	 * @throws RegexException
	 */
	private void err(RegexError error) throws RegexException {
		throw new RegexException(error, Data.iIndex);
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
			Data.chCurrent = 0;
			Data.kMeta = MetaType.END;
			return;
		}
		Data.chCurrent = current();
		if (mapMeta.containsKey(Data.chCurrent)) {
			Data.kMeta = mapMeta.get(Data.chCurrent);// 功能字符
		} else {
			Data.kMeta = MetaType.CHARACTER;// 一般字符
		}
	}

	/**
	 * 判断当前位置不是末尾
	 * 
	 * @return 当前字符是否有效
	 */
	private boolean available() {
		return Data.iIndex < strPattern.length();
	}

	/**
	 * 前进一个字符（look forward）
	 * 
	 */
	private void advance() {
		Data.iIndex++;
	}

	/**
	 * 后退一个字符（look back）
	 * 
	 */
	private void roolback() {
		Data.iIndex--;
	}

	/**
	 * 获得当前字符
	 * 
	 * @return 当前字符
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
 * 分析时使用的堆栈数据
 */
class RegexAnalysisData {
	/**
	 * 当前处理的位置
	 */
	public int iIndex = 0;

	/**
	 * 字符
	 */
	public char chCurrent = 0;

	/**
	 * 字符类型
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