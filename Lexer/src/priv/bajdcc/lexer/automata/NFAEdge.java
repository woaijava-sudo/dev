package priv.bajdcc.lexer.automata;

/**
 * NFA±ß
 * 
 * @author bajdcc
 *
 */
public class NFAEdge {
	/**
	 * Ê¼×´Ì¬
	 */
	public NFAStatus m_Begin;
	
	/**
	 * ÖÕ×´Ì¬
	 */
	public NFAStatus m_End;
	
	/**
	 * Êý¾Ý
	 */
	public NFAEdgeData m_Data = new NFAEdgeData();
}
