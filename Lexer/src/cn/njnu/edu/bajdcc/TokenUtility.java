package cn.njnu.edu.bajdcc;

import java.util.HashMap;

/**
 * @author bajdcc
 * ��������
 */
public class TokenUtility {
	
	/**
	 * ��������
	 */
	public static enum TokenType
	{
		KEYWORD,	//�ؼ���
		VARIABLE,	//������
		BOOLEAN,	//������
		CHARACHER,	//�ַ�
		STRING,		//�ַ���
		INTEGER,	//����
		REAL,		//ʵ��
		EOL,		//��ĩβ
		EOF,		//�ļ�ĩβ
		COMMENT,	//ע��
		LABEL,		//��ǩ
	};
		
	/**
	 * ����ID
	 */
	public static enum TokenID
	{
		// �ؼ���
		AUTO, BOOL, BREAK, CASE, CHAR, CLASS, CONST, CONTINUE,
		DEFAULT, DELETE, DO, DOUBLE, ELSE, ENUM, EXTERN, FALSE, FLOAT, FOR,
		GOTO, IF, INT, LONG, MUTABLE, NAMESPACE, NEW, PRIVATE, PROTECTED, PUBLIC,
		REGISTER, RETURN, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TRUE, TYPEDEF,
		UNION, UNSIGNED, USING, VAR, VOID, VOLATILE, WHILE,

		// ������
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

		// ������
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

		// ��Ԫ������
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
	 * ����
	 */
	private static String[] arrTypes = { "signed", "unsigned", "char",
			"bool", "short", "int", "long", "float", "double", "void", };
	
	/**
	 * �ؼ���
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
