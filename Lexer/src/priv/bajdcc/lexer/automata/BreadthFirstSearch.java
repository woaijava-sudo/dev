package priv.bajdcc.lexer.automata;

import java.util.ArrayList;

/**
 * �����������
 * 
 * @author bajdcc
 * @param T
 *            ״̬����
 */
public class BreadthFirstSearch<Edge, Status> implements
		IBreadthFirstSearch<Edge, Status> {

	/**
	 * ���״̬�ļ���
	 */
	public ArrayList<Status> m_Path = new ArrayList<Status>();

	@Override
	public boolean testEdge(Edge edge) {
		return true;
	}

	@Override
	public void visitBegin(Status status) {

	}

	@Override
	public void visitEnd(Status status) {

	}

}
