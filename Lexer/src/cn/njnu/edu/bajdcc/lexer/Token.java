package cn.njnu.edu.bajdcc.lexer;

import cn.njnu.edu.bajdcc.lexer.TokenUtility.TokenType;

/**
 * µ¥´Ê
 * @author bajdcc
 */
public class Token {
	public TokenType kToken = TokenType.ERROR;
	public Object object = null;
	public int iLine = -1;
	public int iIndex = -1; 
}
