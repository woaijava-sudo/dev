package priv.bajdcc.lexer.regex;

import java.util.ArrayList;

/**
 * ˳��ṹ���֧�ṹ
 * 
 * @author bajdcc
 */
public class Constructure implements IRegexComponent {
	/**
	 * ������Ϊ��֧������Ϊ˳��
	 */
	public boolean m_bBranch = false;

	/**
	 * ���ӽ��
	 */
	public ArrayList<IRegexComponent> m_arrComponents = new ArrayList<IRegexComponent>();

	public Constructure(boolean branch) {
		m_bBranch = branch;
	}

	@Override
	public void visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		for (IRegexComponent component : m_arrComponents) {
			component.visit(visitor);
		}
		visitor.visitEnd(this);
	}
}
