package priv.bajdcc.lexer.automata;

/**
 * �Զ���������
 * 
 * @author bajdcc
 *
 */
public enum EdgeType {
	EPSILON("Epsilon��"),CHARSET("�ַ�����");

	private String name;

	EdgeType(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}
