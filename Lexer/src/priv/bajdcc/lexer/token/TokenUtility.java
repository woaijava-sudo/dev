package priv.bajdcc.lexer.token;

/**
 * ��������
 * 
 * @author bajdcc
 */
public class TokenUtility {

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
