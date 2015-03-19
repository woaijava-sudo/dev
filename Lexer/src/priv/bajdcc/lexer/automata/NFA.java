package priv.bajdcc.lexer.automata;

import java.util.Iterator;

import org.vibur.objectpool.ConcurrentLinkedPool;
import org.vibur.objectpool.PoolService;

import priv.bajdcc.lexer.utility.ObjectFactory;

/**
 * ��ȷ�����Զ���NFA
 * 
 * @author bajdcc
 *
 * @param <Edge>
 *            ������
 * @param <Status>
 *            ״̬����
 */
public class NFA {
	/**
	 * �߶����
	 */
	public PoolService<NFAEdge> m_EdgePool = new ConcurrentLinkedPool<NFAEdge>(
			new ObjectFactory<NFAEdge>() {
				public NFAEdge create() {
					return new NFAEdge();
				};
			}, 1024, 10240, false);

	/**
	 * ״̬�����
	 */
	public PoolService<NFAStatus> m_StatusPool = new ConcurrentLinkedPool<NFAStatus>(
			new ObjectFactory<NFAStatus>() {
				public NFAStatus create() {
					return new NFAStatus();
				};
			}, 1024, 10240, false);

	/**
	 * ��������״̬
	 * 
	 * @param begin
	 *            ��ʼ״̬
	 * @param end
	 *            ����״̬
	 * @return �µı�
	 */
	public NFAEdge connect(NFAStatus begin, NFAStatus end) {
		NFAEdge edge = m_EdgePool.take();// ����һ���±�
		edge.m_Begin = begin;
		edge.m_End = end;
		begin.m_OutEdges.add(edge);// ��ӽ���ʼ�ߵĳ���
		end.m_InEdges.add(edge);// ��ӽ������ߵ����
		return edge;
	}

	/**
	 * �Ͽ�ĳ��״̬��ĳ����
	 * 
	 * @param status
	 *            ĳ״̬
	 * @param edge
	 *            ĳ����
	 */
	public void disconnect(NFAStatus status, NFAEdge edge) {
		status.m_OutEdges.remove(edge);// ��ʼ�ߵĳ��߼���ȥ����ǰ��
		edge.m_End.m_InEdges.remove(edge);// ��ǰ�ߵĽ���״̬����߼���ȥ����ǰ��
		m_EdgePool.restore(edge);
	}

	/**
	 * �Ͽ�ĳ��״̬�����б�
	 * 
	 * @param begin
	 *            ĳ״̬
	 */
	public void disconnect(NFAStatus status) {
		/* ���������� */
		for (Iterator<NFAEdge> it = status.m_InEdges.iterator(); it.hasNext();) {
			NFAEdge edge = it.next();
			disconnect(edge.m_Begin, edge);
		}
		/* ������г��� */
		for (Iterator<NFAEdge> it = status.m_OutEdges.iterator(); it.hasNext();) {
			NFAEdge edge = it.next();
			disconnect(status, edge);
		}
		m_StatusPool.restore(status);
	}
}
