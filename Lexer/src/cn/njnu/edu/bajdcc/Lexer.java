package cn.njnu.edu.bajdcc;

import java.util.ArrayList;

/**
 * @author bajdcc
 * 词法分析器
 */
public class Lexer {
	/**
	 * 单词数组
	 */
	private ArrayList<Token> arrTokens = new ArrayList<Token>();
	/**
	 * 当前单词
	 */
	private Token curToken;
	/**
	 * 输入字符串
	 */
	private String strContext;
	
	public Lexer(String strInput)
	{
		strContext = strInput;
	}	
}
