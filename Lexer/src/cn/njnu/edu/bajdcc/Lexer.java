package cn.njnu.edu.bajdcc;

import java.util.ArrayList;

/**
 * @author bajdcc
 * �ʷ�������
 */
public class Lexer {
	/**
	 * ��������
	 */
	private ArrayList<Token> arrTokens = new ArrayList<Token>();
	/**
	 * ��ǰ����
	 */
	private Token curToken;
	/**
	 * �����ַ���
	 */
	private String strContext;
	
	public Lexer(String strInput)
	{
		strContext = strInput;
	}	
}
