package priv.bajdcc.lexer.regex;

/**
 * ������ʽ�����ӿڣ����ࣩ
 * 
 * @author bajdcc
 */
public interface IRegexComponent {
	/**
	 * �趨��չ������ķ�ʽ
	 * 
	 * @param visitor
	 *            �ݹ�����㷨
	 */
	public void visit(IRegexComponentVisitor visitor);
}
