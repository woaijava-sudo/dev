package priv.bajdcc.lexer.regex;

/**
 * ѭ������
 * 
 * @author bajdcc
 */
public class Repetition implements IRegexComponent {
	/**
	 * ѭ���������ʽ
	 */
	public IRegexComponent m_Component = null;
	
	/**
	 * ѭ����������
	 */
	public int m_iLowerBound = 0;
	
	/**
	 * ѭ����������
	 */
	public int m_iUpperBound = 0;
	
	/**
	 * �Ƿ�̰��ģʽ
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
