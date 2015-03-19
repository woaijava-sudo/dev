package priv.bajdcc.lexer.regex;

import java.util.HashMap;

import priv.bajdcc.lexer.stringify.RegexToString;
import priv.bajdcc.lexer.token.TokenUtility;
import priv.bajdcc.lexer.token.MetaType;
import priv.bajdcc.lexer.automata.ENFAVisitor;
import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;

/**
 * ## 正则表达式分析工具 ##<br/>
 * 用于生成语法树<br/>
 * 语法同一般的正则表达式，只有贪婪模式，没有前/后向匹配， 没有捕获功能，仅用于匹配。
 * 
 * @author bajdcc
 */
public class RegexAnalysis extends RegexStringIterator {

	/**
	 * 表达式树根结点
	 */
	private IRegexComponent m_Expression = null;

	/**
	 * 状态集合Sigma
	 */
	private CharacterMap m_Status = new CharacterMap();

	/**
	 * 字符解析组件
	 */
	private RegexStringUtility m_Utility = new RegexStringUtility(this);

	/**
	 * 遍历算法，AST->ENFA
	 */
	private ENFAVisitor m_ENFAVisitor = null;

	private static HashMap<Character, MetaType> g_mapMeta = new HashMap<Character, MetaType>();

	static {
		for (MetaType meta : MetaType.values()) {
			if (meta.getChar() != 0) {
				g_mapMeta.put(meta.getChar(), meta);
			}
		}
	}

	public RegexAnalysis(String pattern) throws RegexException {
		super(pattern);
		compile();
	}

	/**
	 * ## 编译表达式 ##<br/>
	 * 
	 * @throws RegexException
	 */
	private void compile() throws RegexException {
		translate();
		/* String->AST */
		m_Expression = analysis(MetaType.END.getChar(), MetaType.END);
		/* 重构字符区间 */
		m_Expression.visit(m_Status);
		/* AST->ENFA */
		m_ENFAVisitor = new ENFAVisitor(m_Status);
		m_Expression.visit(m_ENFAVisitor);
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
					if (!charset.addChar(m_Data.m_chCurrent)) {
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
				rep = new Repetition(expression, 0, 1);
				sequence.m_arrComponents.add(rep);
				break;
			case PLUS:// '+'
				next();
				rep = new Repetition(expression, 1, -1);
				sequence.m_arrComponents.add(rep);
				break;
			case STAR:// '*'
				next();
				rep = new Repetition(expression, 0, -1);
				sequence.m_arrComponents.add(rep);
				break;
			case LBRACE: // '{'
				next();
				rep = new Repetition(expression, 0, -1);
				quantity(rep);
				sequence.m_arrComponents.add(rep);
				break;
			default:
				sequence.m_arrComponents.add(expression);
				break;
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
			if (extend) {
				if (TokenUtility.isUpperLetter(ch) || ch == '.') {
					charset.m_bReverse = true;// 大写则取反
				}
				char cl = Character.toLowerCase(ch);
				switch (cl) {
				case 'd':// 数字
					charset.addRange('0', '9');
					return;
				case 'a':// 字母
					charset.addRange('a', 'z');
					charset.addRange('A', 'Z');
					return;
				case 'w':// 标识符
					charset.addRange('a', 'z');
					charset.addRange('A', 'Z');
					charset.addRange('0', '9');
					charset.addChar('_');
					return;
				case 's':// 空白字符
					charset.addChar('\r');
					charset.addChar('\n');
					charset.addChar('\t');
					charset.addChar('\b');
					charset.addChar('\f');
					charset.addChar(' ');
					return;
				default:
					break;
				}
			}
			if (TokenUtility.isLetter(ch)) {// 如果为字母
				ch = m_Utility.fromEscape(ch, RegexError.ESCAPE);
				if (!charset.addChar(ch)) {
					err(RegexError.RANGE);
				}
			}
		} else if (m_Data.m_kMeta == MetaType.END) {
			err(RegexError.INCOMPLETE);
		} else {// 功能字符则转义
			next();
			if (!charset.addChar(ch)) {
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
					if (!charset.addChar(lower)) {
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
		lower = digit();// 循环下界
		if (lower == -1) {
			err(RegexError.BRACE);
		}
		if (m_Data.m_kMeta == MetaType.COMMA) {// ','
			next();
			upper = digit();// 得到循环上界
			if (upper == -1) {
				err(RegexError.BRACE);
			}
		}
		if (upper != -1 && upper < lower) {
			err(RegexError.RANGE);
		}
		expect(MetaType.RBRACE, RegexError.BRACE);
		rep.m_iLowerBound = lower;
		rep.m_iUpperBound = upper;
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
					m_strContext.substring(index, m_Data.m_iIndex), 10);
		} catch (NumberFormatException e) {
			return -1;
		}
	}

	@Override
	protected void transform() {
		if (g_mapMeta.containsKey(m_Data.m_chCurrent)) {
			m_Data.m_kMeta = g_mapMeta.get(m_Data.m_chCurrent);// 功能字符
		} else {
			m_Data.m_kMeta = MetaType.CHARACTER;// 一般字符
		}
	}

	@Override
	public String toString() {
		RegexToString alg = new RegexToString();// 表达式树序列化算法初始化
		m_Expression.visit(alg);// 遍历树
		return alg.toString();
	}

	/**
	 * 获取字符区间描述
	 */
	public String getStatusString() {
		return m_Status.toString();
	}

	/**
	 * 获取ENFA描述
	 */
	public String getENFAString() {
		return m_ENFAVisitor.toString();
	}
}
