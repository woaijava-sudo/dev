package cn.njnu.edu.bajdcc;

import cn.njnu.edu.bajdcc.TokenUtility.TokenType;

/**
 * @author bajdcc
 * ����
 */
public class Token {
	public TokenType kToken = TokenType.EOF;
	public Object object = null;
	public int iLine = -1;
}
