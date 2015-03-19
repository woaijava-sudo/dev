package priv.bajdcc.lexer.token;

/**
 * 单词类型
 */
public enum TokenType {
	KEYWORD("关键字"), ID("标识符"), BOOLEAN("布尔型"), CHARACHER("字符"), STRING(
			"字符串"), INTEGER("整数"), REAL("实数"), EOL("行末尾"), EOF("文件末尾"), COMMENT(
			"注释"), LABEL("标签"), ERROR("错误");

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
