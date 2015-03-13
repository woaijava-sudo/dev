package priv.bajdcc.lexer.token;

import java.util.HashMap;

/**
 * 单词特性
 * 
 * @author bajdcc
 */
public class TokenUtility {
	/**
	 * 单词类型
	 */
	public enum TokenType {
		KEYWORD("关键字"), ID("标识符"), BOOLEAN("布尔型"), CHARACHER("字符"), STRING(
				"字符串"), INTEGER("整数"), REAL("实数"), EOL("行末尾"), EOF("文件末尾"), COMMENT(
				"注释"), LABEL("标签"), ERROR("错误");

		private String name;

		TokenType(String name) {
			this.name = name;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}
	};

	/**
	 * 关键词
	 */
	public enum KeywordType {
		AUTO("auto"), BOOL("bool"), BREAK("break"), CASE("case"), CHAR("char"), CLASS(
				"class"), CONST("const"), CONTINUE("continue"), DEFAULT(
				"default"), DELETE("delete"), DO("do"), DOUBLE("double"), ELSE(
				"else"), ENUM("enum"), EXTERN("extern"), FALSE("false"), FLOAT(
				"float"), FOR("for"), GOTO("goto"), IF("if"), INT("int"), LONG(
				"long"), MUTABLE("mutable"), NAMESPACE("namespace"), NEW("new"), PRIVATE(
				"private"), PROTECTED("protected"), PUBLIC("public"), REGISTER(
				"register"), RETURN("return"), SIGNED("signed"), SIZEOF(
				"sizeof"), STATIC("static"), STRUCT("struct"), SWITCH("switch"), TRUE(
				"true"), TYPEDEF("typedef"), UNION("union"), UNSIGNED(
				"unsigned"), USING("using"), VAR("var"), VOID("void"), VOLATILE(
				"volatile"), WHILE("while");

		private String name;

		KeywordType(String name) {
			this.name = name;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}
	};

	/**
	 * 操作符
	 */
	public enum OperatorType {
		ASSIGN("="), PLUS("+"), MINUS("-"), TIMES("*"), DIV("/"), MOD("%"), BIT_AND(
				"&"), BIT_OR("|"), BIT_NOT("~"), LOG_NOT("!"), LT("<"), GT(">"),

		LPARAN("("), RPARAN(")"), LBRACE("{"), RBRACE("}"), LSQUARE("["), RSQUARE(
				"]"), COMMA(","), DOT("."), SEMI(";"), COLON(":"),

		EQ("=="), NEQ("!="), PLUS_PLUS("++"), MINUS_MINUS("--"), PLUS_ASSIGN(
				"+="), MINUS_ASSIGN("-="), TIMES_ASSIGN("*="), DIV_ASSIGN("/="), NGT(
				"<="), NLT(">="), LOGICAL_AND("&&"), LOGICAL_OR("||");

		private String name;

		OperatorType(String name) {
			this.name = name;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}
	}

	/**
	 * 数据类型
	 */
	public enum DataType {
		SIGNED("signed"), UNSIGNED("unsigned"), CHAR("char"), BOOL("bool"), SHORT(
				"short"), INT("int"), LONG("long"), FLOAT("float"), DOUBLE(
				"double"), VOID("void");

		private String name;

		DataType(String name) {
			this.name = name;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}
	}

	/**
	 * 字符类型
	 */
	public enum MetaType {
		CHARACTER('\0'), LPARAN('('), RPARAN(')'), CARET('^'), DOLLAR('$'), STAR(
				'*'), PLUS('+'), QUERY('?'), LSQARE('['), RSQARE(']'), BAR('|'), ESCAPE(
				'\\'), DASH('-'), LBRACE('{'), RBRACE('}'), COMMA(','), NEW_LINE(
				'\n'), CARRIAGE_RETURN('\r'), BACKSPACE('\b'), END('\0');

		private char ch;

		MetaType(char ch) {
			this.ch = ch;
		}

		public char getChar() {
			return ch;
		}

		public void setChar(char ch) {
			this.ch = ch;
		}
	}

	/**
	 * 是否为ASCII英文字母
	 * 
	 * @param ch
	 *            字符
	 * @return 是否为ASCII英文字母
	 */
	public static boolean isLetter(char ch) {
		return isUpperLetter(ch) || isLowerLetter(ch);
	}

	/**
	 * 是否为ASCII大写英文字母
	 * 
	 * @param ch
	 *            字符
	 * @return 是否为ASCII大写英文字母
	 */
	public static boolean isUpperLetter(char ch) {
		return ch >= 'A' && ch <= 'Z';
	}

	/**
	 * 是否为ASCII小写英文字母
	 * 
	 * @param ch
	 *            字符
	 * @return 是否为ASCII小写英文字母
	 */
	public static boolean isLowerLetter(char ch) {
		return ch >= 'a' && ch <= 'z';
	}
}
