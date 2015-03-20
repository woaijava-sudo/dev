package priv.bajdcc.lexer.automata.nfa;

import priv.bajdcc.lexer.automata.EdgeType;

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
