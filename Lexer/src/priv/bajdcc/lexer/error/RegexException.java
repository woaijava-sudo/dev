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
		RANGE("��Χ����ȷ"),
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
		m_iLocation = location;
		m_kError = error;
	}

	/**
	 * λ��
	 */
	private int m_iLocation = -1;

	/**
	 * @return ����λ��
	 */
	public int getLocation() {
		return m_iLocation;
	}

	/**
	 * ��������
	 */
	private RegexError m_kError = RegexError.NULL;

	/**
	 * @return ��������
	 */
	public RegexError getErrorCode() {
		return m_kError;
	}
}
