package priv.bajdcc.lexer.token;

import java.util.HashMap;

/**
 * ��������
 * 
 * @author bajdcc
 */
public class TokenUtility {
	/**
	 * ��������
	 */
	public enum TokenType {
		KEYWORD("�ؼ���"), ID("��ʶ��"), BOOLEAN("������"), CHARACHER("�ַ�"), STRING(
				"�ַ���"), INTEGER("����"), REAL("ʵ��"), EOL("��ĩβ"), EOF("�ļ�ĩβ"), COMMENT(
				"ע��"), LABEL("��ǩ"), ERROR("����");

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
	 * �ؼ���
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
	 * ������
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
	 * ��������
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
	 * �ַ�����
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
	 * �Ƿ�ΪASCIIӢ����ĸ
	 * 
	 * @param ch
	 *            �ַ�
	 * @return �Ƿ�ΪASCIIӢ����ĸ
	 */
	public static boolean isLetter(char ch) {
		return isUpperLetter(ch) || isLowerLetter(ch);
	}

	/**
	 * �Ƿ�ΪASCII��дӢ����ĸ
	 * 
	 * @param ch
	 *            �ַ�
	 * @return �Ƿ�ΪASCII��дӢ����ĸ
	 */
	public static boolean isUpperLetter(char ch) {
		return ch >= 'A' && ch <= 'Z';
	}

	/**
	 * �Ƿ�ΪASCIIСдӢ����ĸ
	 * 
	 * @param ch
	 *            �ַ�
	 * @return �Ƿ�ΪASCIIСдӢ����ĸ
	 */
	public static boolean isLowerLetter(char ch) {
		return ch >= 'a' && ch <= 'z';
	}
}
