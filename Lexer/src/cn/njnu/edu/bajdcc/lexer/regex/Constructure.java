package cn.njnu.edu.bajdcc.lexer.regex;

import java.util.ArrayList;

/**
 * 顺序结构或分支结构
 * 
 * @author bajdcc
 */
public class Constructure implements IRegexComponent {
	/**
	 * 若是则为分支，否则为顺序
	 */
	public boolean bBranch = true;

	/**
	 * 孩子结点
	 */
	public ArrayList<IRegexComponent> Components = new ArrayList<IRegexComponent>();

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		for (IRegexComponent component : Components) {
			component.Visit(visitor);
		}
		visitor.visitEnd(this);
	}
}
