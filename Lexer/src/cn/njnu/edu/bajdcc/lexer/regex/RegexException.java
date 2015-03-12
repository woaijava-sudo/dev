package cn.njnu.edu.bajdcc.lexer.regex;

/**
 * ������ʽ���ɹ����е��쳣
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
	 * λ��
	 */
	private int iLocation = -1;

	public int getLocation() {
		return iLocation;
	}
}
