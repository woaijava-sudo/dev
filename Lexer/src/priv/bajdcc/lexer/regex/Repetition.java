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

	public Repetition() {

	}

	public Repetition(IRegexComponent component, int begin, int end) {
		m_Component = component;
		m_iLowerBound = begin;
		m_iUpperBound = end;
	}

	@Override
	public void visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		m_Component.visit(visitor);
		visitor.visitEnd(this);
	}
}
