package priv.bajdcc.lexer.automata.dfa;

import java.util.ArrayList;
import java.util.Arrays;

import priv.bajdcc.lexer.automata.nfa.NFAStatus;
import priv.bajdcc.lexer.automata.nfa.NFAStatusData;

/**
 * DFA״̬����
 * 
 * @author bajdcc
 *
 */
public class DFAStatusData extends NFAStatusData {
	/**
	 * NFA״̬����
	 */
	public ArrayList<NFAStatus> m_NFAStatus = new ArrayList<NFAStatus>();

	/**
	 * ���״̬������������ŷָ���
	 */
	public String getStatusString(ArrayList<NFAStatus> nfaStatusList) {
		if (m_NFAStatus.isEmpty()) {
			return "";
		}
		int[] orders = new int[m_NFAStatus.size()];
		for (int i = 0; i < orders.length; i++) {
			orders[i] = nfaStatusList.indexOf(m_NFAStatus.get(i));
		}
		Arrays.sort(orders);
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < orders.length; i++) {
			sb.append(orders[i] + ",");
		}
		return sb.toString();
	}
}
