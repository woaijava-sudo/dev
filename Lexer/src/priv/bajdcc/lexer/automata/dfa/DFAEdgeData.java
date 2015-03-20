package priv.bajdcc.lexer.automata.dfa;

import java.util.ArrayList;

import priv.bajdcc.lexer.automata.nfa.NFAEdge;
import priv.bajdcc.lexer.automata.nfa.NFAEdgeData;

/**
 * DFA������
 * 
 * @author bajdcc
 *
 */
public class DFAEdgeData extends NFAEdgeData {
	/**
	 * NFA�߼���
	 */
	public ArrayList<NFAEdge> m_NFAEdges = new ArrayList<NFAEdge>();
}
