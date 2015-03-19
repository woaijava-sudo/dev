package priv.bajdcc.lexer.token;

/**
 * ��������
 */
public enum TokenType {
	KEYWORD("�ؼ���"), ID("��ʶ��"), BOOLEAN("������"), CHARACHER("�ַ�"), STRING(
			"�ַ���"), INTEGER("����"), REAL("ʵ��"), EOL("��ĩβ"), EOF("�ļ�ĩβ"), COMMENT(
			"ע��"), LABEL("��ǩ"), ERROR("����");

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
