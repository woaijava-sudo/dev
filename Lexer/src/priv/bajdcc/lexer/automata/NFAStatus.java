package priv.bajdcc.lexer.automata;

import java.util.ArrayList;

/**
 * NFA״̬
 * 
 * @author bajdcc
 *
 */
public class NFAStatus {
	/**
	 * ���߼���
	 */
	public ArrayList<NFAEdge> m_OutEdges = new ArrayList<NFAEdge>();

	/**
	 * ��߼���
	 */
	public ArrayList<NFAEdge> m_InEdges = new ArrayList<NFAEdge>();

	/**
	 * ����
	 */
	public NFAStatusData m_Data = new NFAStatusData();

	/**
	 * ���������������״̬������״̬
	 * 
	 * @param bfs
	 *            �����㷨
	 */
	public void visit(BreadthFirstSearch<NFAEdge, NFAStatus> bfs) {
		ArrayList<NFAStatus> stack = bfs.m_Path;
		stack.clear();
		stack.add(this);
		for (int i = 0; i < stack.size(); i++) {// ����ÿ��״̬
			NFAStatus status = stack.get(i);
			bfs.visitBegin(status);
			for (NFAEdge edge : status.m_OutEdges) {// ����״̬�ĳ���
				if (!stack.contains(edge.m_End) && bfs.testEdge(edge)) {
					stack.add(edge.m_End);
				}
			}
			bfs.visitEnd(status);
		}
	}
}
