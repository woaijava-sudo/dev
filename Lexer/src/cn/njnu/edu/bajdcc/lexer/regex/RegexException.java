package cn.njnu.edu.bajdcc.lexer.regex;

/**
 * 正则表达式生成过程中的异常
 * 
 * @author bajdcc
 *
 */
public class RegexException extends Exception {

	/**
	 * 
	 */
	private static final long serialVersionUID = 51820185044284191L;

	public RegexException(String message, int location) {
		super(message);
		iLocation = location;
	}

	/**
	 * 位置
	 */
	private int iLocation = -1;

	public int getLocation() {
		return iLocation;
	}
}
