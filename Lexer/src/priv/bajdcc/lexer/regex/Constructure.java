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
	public boolean bBranch = false;

	/**
	 * ���ӽ��
	 */
	public ArrayList<IRegexComponent> Components = new ArrayList<IRegexComponent>();

	public Constructure(boolean branch) {
		bBranch = branch;
	}

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		for (IRegexComponent component : Components) {
			component.Visit(visitor);
		}
		visitor.visitEnd(this);
	}
}
