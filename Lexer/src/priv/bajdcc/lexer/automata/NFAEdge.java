package priv.bajdcc.lexer.automata;

/**
 * NFA��
 * 
 * @author bajdcc
 *
 */
public class NFAEdge {
	/**
	 * ʼ״̬
	 */
	public NFAStatus m_Begin;
	
	/**
	 * ��״̬
	 */
	public NFAStatus m_End;
	
	/**
	 * ����
	 */
	public NFAEdgeData m_Data = new NFAEdgeData();
}
