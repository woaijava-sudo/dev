package priv.bajdcc.lexer.automata.dfa;

import java.util.ArrayList;
import java.util.HashSet;

import priv.bajdcc.lexer.automata.nfa.NFAStatus;

/**
 * DFA�߸������ݽṹ
 * @author bajdcc
 *
 */
public class DFAEdgeBag extends DFAEdgeData{
	/**
	 * NFA״̬����
	 */
	public HashSet<NFAStatus> m_NFAStatus = new HashSet<NFAStatus>();
	
	/**
	 * ���״̬������������ŷָ���
	 */
	public String getStatusString(ArrayList<NFAStatus> nfaStatusList) {
		StringBuilder sb = new StringBuilder();
		for (NFAStatus status : m_NFAStatus) {
			sb.append(nfaStatusList.indexOf(status) + ",");
		}
		return sb.toString();
	}
}
