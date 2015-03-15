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
	public IRegexComponent m_Component = null;
	
	/**
	 * 循环次数下限
	 */
	public int m_iLowerBound = 0;
	
	/**
	 * 循环次数上限
	 */
	public int m_iUpperBound = 0;
	
	/**
	 * 是否贪婪模式
	 */
	public boolean m_bGreedy = false;

	public Repetition() {

	}

	public Repetition(IRegexComponent component, int begin, int end,
			boolean greedy) {
		m_Component = component;
		m_iLowerBound = begin;
		m_iUpperBound = end;
		m_bGreedy = greedy;
	}

	@Override
	public void visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		m_Component.visit(visitor);
		visitor.visitEnd(this);
	}
}
