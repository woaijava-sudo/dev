package cn.njnu.edu.bajdcc;

import java.util.HashMap;

/**
 * @author bajdcc
 * 单词特性
 */
public class TokenUtility {
	
	/**
	 * 单词类型
	 */
	public static enum TokenType
	{
		KEYWORD,	//关键字
		VARIABLE,	//变量名
		BOOLEAN,	//布尔型
		CHARACHER,	//字符
		STRING,		//字符串
		INTEGER,	//整数
		REAL,		//实数
		EOL,		//行末尾
		EOF,		//文件末尾
		COMMENT,	//注释
		LABEL,		//标签
	};
		
	/**
	 * 单词ID
	 */
	public static enum TokenID
	{
		// 关键字
		AUTO, BOOL, BREAK, CASE, CHAR, CLASS, CONST, CONTINUE,
		DEFAULT, DELETE, DO, DOUBLE, ELSE, ENUM, EXTERN, FALSE, FLOAT, FOR,
		GOTO, IF, INT, LONG, MUTABLE, NAMESPACE, NEW, PRIVATE, PROTECTED, PUBLIC,
		REGISTER, RETURN, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TRUE, TYPEDEF,
		UNION, UNSIGNED, USING, VAR, VOID, VOLATILE, WHILE,

		// 操作符
		ASSIGN				/* = */,
		PLUS				/* + */,
		MINUS				/* - */,
		TIMES				/* * */,
		DIV					/* / */,
		MOD					/* % */,
		BIT_AND				/* & */,
		BIT_OR				/* | */,
		BIT_NOT				/* ~ */,
		LOG_NOT				/* ! */,
		LT					/* < */,
		GT					/* > */,

		// 标点符号
		LPARAN				/* ( */,
		RPARAN				/* ) */,
		LBRACE				/* { */,
		RBRACE				/* } */,
		LSQUARE				/* [ */,
		RSQUARE				/* ] */,
		COMMA				/* , */,
		DOT					/* . */,
		SEMI				/* ; */,
		COLON				/* : */,

		// 二元操作符
		EQ					/* == */,
		NEQ					/* != */,
		PLUS_PLUS			/* ++ */,
		MINUS_MINUS			/* -- */,
		PLUS_ASSIGN			/* += */,
		MINUS_ASSIGN		/* -= */,
		TIMES_ASSIGN		/* *= */,
		DIV_ASSIGN			/* /= */,
		NGT					/* <= */,
		NLT					/* >= */,
		LOGICAL_AND			/* && */,
		LOGICAL_OR			/* || */,
};
	
	
	/**
	 * 类型
	 */
	private static String[] arrTypes = { "signed", "unsigned", "char",
			"bool", "short", "int", "long", "float", "double", "void", };
	
	/**
	 * 关键词
	 */
	private static String[] arrKeywords = { "auto", "bool", "break", "case",
			"char", "class", "const", "continue", "default", "delete", "do",
			"double", "else", "enum", "extern", "false", "float", "for",
			"goto", "if", "int", "long", "mutable", "namespace", "new",
			"private", "protected", "public", "register", "return", "signed",
			"sizeof", "static", "struct", "switch", "true", "typedef", "union",
			"unsigned", "using", "var", "void", "volatile", "while", "=", "+",
			"-", "*", "/", "%", "&", "|", "~", "!", "<", ">", "(", ")", "{",
			"}", "[", "]", ",", ".", ";", ":", "==", "!=", "++", "--", "+=",
			"-=", "*=", "/=", "<=", ">=", "&&", "||"};
	
	private static HashMap<String, TokenID> mapKeywords = new HashMap<String, TokenID>();
	
	static {
		for (int i = 0; i < arrKeywords.length; i++) {
			mapKeywords.put(arrKeywords[i], TokenID.values()[i]);
		}
	}
}
