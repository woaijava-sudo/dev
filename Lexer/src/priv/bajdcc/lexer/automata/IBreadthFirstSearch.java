package priv.bajdcc.lexer.automata;

/**
 * 
 * BFS������ȱ����ӿ�
 * 
 * @param <Edge>
 *            ������
 * @param <Status>
 *            ״̬����
 * @author bajdcc
 *
 */
public interface IBreadthFirstSearch<Edge, Status> {
	/**
	 * �߲���
	 * 
	 * @return ���Խ��
	 */
	public boolean testEdge(Edge edge);

	/**
	 * ������ʼ
	 * 
	 * @param status
	 *            ״̬
	 */
	public void visitBegin(Status status);

	/**
	 * ��������
	 * 
	 * @param status
	 *            ״̬
	 */
	public void visitEnd(Status status);
}
