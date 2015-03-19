package priv.bajdcc.lexer.automata;

import java.util.Iterator;

import org.vibur.objectpool.ConcurrentLinkedPool;
import org.vibur.objectpool.PoolService;

import priv.bajdcc.lexer.utility.ObjectFactory;

/**
 * 非确定性自动机NFA
 * 
 * @author bajdcc
 *
 * @param <Edge>
 *            边类型
 * @param <Status>
 *            状态类型
 */
public class NFA {
	/**
	 * 边对象池
	 */
	public PoolService<NFAEdge> m_EdgePool = new ConcurrentLinkedPool<NFAEdge>(
			new ObjectFactory<NFAEdge>() {
				public NFAEdge create() {
					return new NFAEdge();
				};
			}, 1024, 10240, false);

	/**
	 * 状态对象池
	 */
	public PoolService<NFAStatus> m_StatusPool = new ConcurrentLinkedPool<NFAStatus>(
			new ObjectFactory<NFAStatus>() {
				public NFAStatus create() {
					return new NFAStatus();
				};
			}, 1024, 10240, false);

	/**
	 * 连接两个状态
	 * 
	 * @param begin
	 *            初始状态
	 * @param end
	 *            结束状态
	 * @return 新的边
	 */
	public NFAEdge connect(NFAStatus begin, NFAStatus end) {
		NFAEdge edge = m_EdgePool.take();// 申请一条新边
		edge.m_Begin = begin;
		edge.m_End = end;
		begin.m_OutEdges.add(edge);// 添加进起始边的出边
		end.m_InEdges.add(edge);// 添加进结束边的入边
		return edge;
	}

	/**
	 * 断开某个状态和某条边
	 * 
	 * @param status
	 *            某状态
	 * @param edge
	 *            某条边
	 */
	public void disconnect(NFAStatus status, NFAEdge edge) {
		status.m_OutEdges.remove(edge);// 起始边的出边集合去除当前边
		edge.m_End.m_InEdges.remove(edge);// 当前边的结束状态的入边集合去除当前边
		m_EdgePool.restore(edge);
	}

	/**
	 * 断开某个状态和所有边
	 * 
	 * @param begin
	 *            某状态
	 */
	public void disconnect(NFAStatus status) {
		/* 清除所有入边 */
		for (Iterator<NFAEdge> it = status.m_InEdges.iterator(); it.hasNext();) {
			NFAEdge edge = it.next();
			disconnect(edge.m_Begin, edge);
		}
		/* 清除所有出边 */
		for (Iterator<NFAEdge> it = status.m_OutEdges.iterator(); it.hasNext();) {
			NFAEdge edge = it.next();
			disconnect(status, edge);
		}
		m_StatusPool.restore(status);
	}
}
