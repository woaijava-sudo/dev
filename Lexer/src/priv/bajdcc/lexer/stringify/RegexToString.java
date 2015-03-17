package priv.bajdcc.lexer.stringify;

import priv.bajdcc.lexer.regex.Charset;
import priv.bajdcc.lexer.regex.Constructure;
import priv.bajdcc.lexer.regex.IRegexComponentVisitor;
import priv.bajdcc.lexer.regex.Repetition;

public class RegexToString implements IRegexComponentVisitor {

	/**
	 * ������ʽ�����ͱ��
	 */
	private StringBuilder m_Context = new StringBuilder();

	/**
	 * ǰ׺
	 */
	private StringBuilder m_Prefix = new StringBuilder();

	/**
	 * ǰ׺����
	 */
	private void appendPrefix() {
		m_Prefix.append('\t');
		m_Context.append(" {" + System.getProperty("line.separator"));
	}

	/**
	 * ȡ��ǰ׺����
	 */
	private void reducePrefix() {
		m_Prefix.deleteCharAt(0);
		m_Context.append(m_Prefix + "}" + System.getProperty("line.separator"));
	}

	@Override
	public void visitBegin(Charset node) {
		m_Context.append(m_Prefix);
		switch (node.m_kChar) {
		case BEGIN:
			m_Context.append("����");
			break;
		case END:
			m_Context.append("��ĩ");
			break;
		case NORMAL:
			m_Context.append("�ַ�");
			m_Context.append((node.m_bReverse ? "[ȡ��]" : "") + "\t" + node);
			break;
		default:
			break;
		}
		m_Context.append(System.getProperty("line.separator"));
	}

	@Override
	public void visitBegin(Constructure node) {
		m_Context.append(m_Prefix.toString() + (node.m_bBranch ? "��֧" : "����"));
		appendPrefix();
	}

	@Override
	public void visitBegin(Repetition node) {
		m_Context.append(m_Prefix.toString() + "ѭ��["
				+ (node.m_bGreedy ? "" : "��") + "̰��]{" + node.m_iLowerBound
				+ "," + node.m_iUpperBound + "}");
		appendPrefix();
	}

	@Override
	public void visitEnd(Charset node) {

	}

	@Override
	public void visitEnd(Constructure node) {
		reducePrefix();
	}

	@Override
	public void visitEnd(Repetition node) {
		reducePrefix();
	}

	@Override
	public String toString() {
		return m_Context.toString();
	}
}
