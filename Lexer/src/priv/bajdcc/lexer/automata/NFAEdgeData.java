package priv.bajdcc.lexer.automata;

/**
 * NFA������
 * 
 * @author bajdcc
 *
 */
public class NFAEdgeData {
	/**
	 * ������
	 */
	public EdgeType m_Action = EdgeType.EPSILON;
	
	/**
	 * ����
	 */
	public int m_Param = -1;
}
