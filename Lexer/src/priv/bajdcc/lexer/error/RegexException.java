package priv.bajdcc.lexer.error;

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

	/**
	 * ������ʽ���������еĴ���
	 */
	public enum RegexError {
		NULL("������ʽΪ��"),
		CTYPE("�Ƿ��ַ�"),
		ESCAPE("�Ƿ���ת���ַ�"),
		BRACK("�����Ų�ƥ��"),
		PAREN("С���Ų�ƥ��"),
		BRACE("�����Ų�ƥ��"),
		BADBRACE("���ַ�Χ����ȷ"),
		RANGE("�ַ���Χ����ȷ"),
		BADREPEAT("ν��ʹ�ô���"),
		SYNTAX("�﷨����"),
		INCOMPLETE("������ʽ������");

		private String message;

		RegexError(String message) {
			this.message = message;
		}

		public String getMessage() {
			return message;
		}

		public void setMessage(String message) {
			this.message = message;
		}
	};

	public RegexException(RegexError error, int location) {
		super(error.getMessage());
		iLocation = location;
		kError = error;
	}

	/**
	 * λ��
	 */
	private int iLocation = -1;

	/**
	 * @return ����λ��
	 */
	public int getLocation() {
		return iLocation;
	}

	/**
	 * ��������
	 */
	private RegexError kError = RegexError.NULL;

	/**
	 * @return ��������
	 */
	public RegexError getErrorCode() {
		return kError;
	}
}
