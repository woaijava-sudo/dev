package priv.bajdcc.lexer.regex;

/**
 * 循环功能
 * 
 * @author bajdcc
 */
public class Repetition implements IRegexComponent {
	/**
	 * 循环部件表达式
	 */
	public IRegexComponent Component;
	/**
	 * 循环次数下限
	 */
	public int iBegin = 0;
	/**
	 * 循环次数上限
	 */
	public int iEnd = 0;
	/**
	 * 是否贪婪模式
	 */
	public boolean bGreed = false;

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		Component.Visit(visitor);
		visitor.visitEnd(this);
	}
}
