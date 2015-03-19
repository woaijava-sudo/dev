package priv.bajdcc.lexer.automata;

import java.util.ArrayList;

/**
 * NFA状态
 * 
 * @author bajdcc
 *
 */
public class NFAStatus {
	/**
	 * 出边集合
	 */
	public ArrayList<NFAEdge> m_OutEdges = new ArrayList<NFAEdge>();

	/**
	 * 入边集合
	 */
	public ArrayList<NFAEdge> m_InEdges = new ArrayList<NFAEdge>();

	/**
	 * 数据
	 */
	public NFAStatusData m_Data = new NFAStatusData();

	/**
	 * 遍历自身，添加自身状态及其子状态
	 * 
	 * @param bfs
	 *            遍历算法
	 */
	public void visit(BreadthFirstSearch<NFAEdge, NFAStatus> bfs) {
		ArrayList<NFAStatus> stack = bfs.m_Path;
		stack.clear();
		stack.add(this);
		for (int i = 0; i < stack.size(); i++) {// 遍历每个状态
			NFAStatus status = stack.get(i);
			bfs.visitBegin(status);
			for (NFAEdge edge : status.m_OutEdges) {// 遍历状态的出边
				if (!stack.contains(edge.m_End) && bfs.testEdge(edge)) {
					stack.add(edge.m_End);
				}
			}
			bfs.visitEnd(status);
		}
	}
}
