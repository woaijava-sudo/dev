package priv.bajdcc.lexer.automata.dfa;

/**
 * DFA��
 * 
 * @author bajdcc
 *
 */
public class DFAEdge {
	/**
	 * ��̬
	 */
	public DFAStatus m_Begin;

	/**
	 * ��̬
	 */
	public DFAStatus m_End;

	/**
	 * ����
	 */
	public DFAEdgeData m_Data = new DFAEdgeData();
}