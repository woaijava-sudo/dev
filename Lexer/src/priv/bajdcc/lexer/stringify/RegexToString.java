package priv.bajdcc.lexer.stringify;

import priv.bajdcc.lexer.regex.Charset;
import priv.bajdcc.lexer.regex.Constructure;
import priv.bajdcc.lexer.regex.IRegexComponentVisitor;
import priv.bajdcc.lexer.regex.Repetition;

public class RegexToString implements IRegexComponentVisitor {

	/**
	 * 正则表达式的树型表达
	 */
	private StringBuilder m_Context = new StringBuilder();

	/**
	 * 前缀
	 */
	private StringBuilder m_Prefix = new StringBuilder();

	/**
	 * 前缀缩进
	 */
	private void appendPrefix() {
		m_Prefix.append('\t');
		m_Context.append(" {" + System.getProperty("line.separator"));
	}

	/**
	 * 取消前缀缩进
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
			m_Context.append("行首");
			break;
		case END:
			m_Context.append("行末");
			break;
		case NORMAL:
			m_Context.append("字符");
			m_Context.append((node.m_bReverse ? "[取反]" : "") + "\t" + node);
			break;
		default:
			break;
		}
		m_Context.append(System.getProperty("line.separator"));
	}

	@Override
	public void visitBegin(Constructure node) {
		m_Context.append(m_Prefix.toString() + (node.m_bBranch ? "分支" : "序列"));
		appendPrefix();
	}

	@Override
	public void visitBegin(Repetition node) {
		m_Context.append(m_Prefix.toString() + "循环["
				+ (node.m_bGreedy ? "" : "非") + "贪婪]{" + node.m_iLowerBound
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
