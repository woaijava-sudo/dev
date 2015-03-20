package priv.bajdcc.lexer.token;

/**
 * ��������
 * 
 * @author bajdcc
 */
public enum TokenType {
	KEYWORD("�ؼ���"), ID("��ʶ��"), WHITESPACE("�հ��ַ�"), CHARACTER("�ַ�"), STRING(
			"�ַ���"), INTEGER("����"), REAL("ʵ��"), EOF("ȫ��ĩβ"), COMMENT("ע��"), OPERATOR(
			"������"), ERROR("����");

	private String name;

	TokenType(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
};
