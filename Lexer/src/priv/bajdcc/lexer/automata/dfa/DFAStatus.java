package priv.bajdcc.lexer.automata.dfa;

import java.util.ArrayList;

import priv.bajdcc.lexer.automata.BreadthFirstSearch;

/**
 * DFA״̬
 * 
 * @author bajdcc
 *
 */
public class DFAStatus {
	/**
	 * ���߼���
	 */
	public ArrayList<DFAEdge> m_OutEdges = new ArrayList<DFAEdge>();

	/**
	 * ��߼���
	 */
	public ArrayList<DFAEdge> m_InEdges = new ArrayList<DFAEdge>();

	/**
	 * ����
	 */
	public DFAStatusData m_Data = new DFAStatusData();
	
	/**
	 * ���ڱ���������״̬���ڵ�����״̬����ͨ������������PATH��
	 * 
	 * @param bfs
	 *            �����㷨
	 */
	public void visit(BreadthFirstSearch<DFAEdge, DFAStatus> bfs) {
		ArrayList<DFAStatus> stack = bfs.m_Path;
		stack.clear();
		stack.add(this);
		for (int i = 0; i < stack.size(); i++) {// ����ÿ��״̬
			DFAStatus status = stack.get(i);
			bfs.visitBegin(status);
			for (DFAEdge edge : status.m_OutEdges) {// ����״̬�ĳ���
				if (!stack.contains(edge.m_End) && bfs.testEdge(edge)) {// ��δ�����ʣ��ұ����ͷ���Ҫ��
					stack.add(edge.m_End);
				}
			}
			bfs.visitEnd(status);
		}
	}
}
