package priv.bajdcc.lexer.automata;

/**
 * NFA边数据
 * 
 * @author bajdcc
 *
 */
public class NFAEdgeData {
	/**
	 * 边类型
	 */
	public EdgeType m_Action = EdgeType.EPSILON;
	
	/**
	 * 数据
	 */
	public int m_Param = -1;
}
