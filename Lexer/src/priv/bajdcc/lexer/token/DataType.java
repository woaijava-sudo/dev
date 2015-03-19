package priv.bajdcc.lexer.token;

/**
 * ��������
 */
public enum DataType {
	SIGNED("signed"), UNSIGNED("unsigned"), CHAR("char"), BOOL("bool"), SHORT(
			"short"), INT("int"), LONG("long"), FLOAT("float"), DOUBLE(
			"double"), VOID("void");

	private String name;

	DataType(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
