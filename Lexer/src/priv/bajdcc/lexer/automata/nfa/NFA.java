package priv.bajdcc.lexer.automata.nfa;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.Stack;

import org.vibur.objectpool.ConcurrentLinkedPool;
import org.vibur.objectpool.PoolService;

import priv.bajdcc.lexer.automata.BreadthFirstSearch;
import priv.bajdcc.lexer.automata.EdgeType;
import priv.bajdcc.lexer.regex.CharacterMap;
import priv.bajdcc.lexer.regex.CharacterRange;
import priv.bajdcc.lexer.regex.Charset;
import priv.bajdcc.lexer.regex.Constructure;
import priv.bajdcc.lexer.regex.IRegexComponent;
import priv.bajdcc.lexer.regex.IRegexComponentVisitor;
import priv.bajdcc.lexer.regex.Repetition;
import priv.bajdcc.lexer.utility.ObjectFactory;

/**
 * NFA�����㷨��AST->NFA��
 * 
 * @author bajdcc
 *
 */
public class NFA implements IRegexComponentVisitor {

	/**
	 * �Ƿ�Ϊ����ģʽ����ӡ��Ϣ��
	 */
	protected boolean m_bDebug = false;
	
	/**
	 * �߶����
	 */
	private PoolService<NFAEdge> m_EdgesPool = new ConcurrentLinkedPool<NFAEdge>(
			new ObjectFactory<NFAEdge>() {
				public NFAEdge create() {
					return new NFAEdge();
				};
			}, 1024, 10240, false);

	/**
	 * ״̬�����
	 */
	private PoolService<NFAStatus> m_StatusPool = new ConcurrentLinkedPool<NFAStatus>(
			new ObjectFactory<NFAStatus>() {
				public NFAStatus create() {
					return new NFAStatus();
				};
			}, 1024, 10240, false);

	/**
	 * ���
	 */
	private int m_iLevel = 0;

	/**
	 * NFAջ
	 */
	private Stack<ArrayList<ENFA>> m_stkNFA = new Stack<ArrayList<ENFA>>();

	/**
	 * NFA�ӱ�
	 */
	private ArrayList<ENFA> m_childNFA = new ArrayList<ENFA>();

	/**
	 * ENFA
	 */
	protected ENFA m_mainNFA = null;

	/**
	 * ���ʽ�������
	 */
	protected IRegexComponent m_Expression = null;

	/**
	 * Sigma״̬��
	 */
	protected CharacterMap m_Map = new CharacterMap();

	public NFA(IRegexComponent exp, boolean debug) {
		m_bDebug = debug;
		m_Expression = exp;
		m_Expression.visit(m_Map);
		if (m_bDebug) {
			System.out.println("#### ״̬���� ####");
			System.out.println(getStatusString());
		}
		m_Expression.visit(this);
		if (m_bDebug) {
			System.out.println("#### EpsilonNFA ####");
			System.out.println(getNFAString());
		}
	}

	/**
	 * ��������״̬
	 * 
	 * @param begin
	 *            ��̬
	 * @param end
	 *            ��̬
	 * @return �µı�
	 */
	protected NFAEdge connect(NFAStatus begin, NFAStatus end) {
		NFAEdge edge = m_EdgesPool.take();// ����һ���±�
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
	protected void disconnect(NFAStatus status, NFAEdge edge) {
		edge.m_End.m_InEdges.remove(edge);// ��ǰ�ߵĽ���״̬����߼���ȥ����ǰ��
		m_EdgesPool.restore(edge);
	}

	/**
	 * �Ͽ�ĳ��״̬�����б�
	 * 
	 * @param begin
	 *            ĳ״̬
	 */
	protected void disconnect(NFAStatus status) {
		/* ���������� */
		for (Iterator<NFAEdge> it = status.m_InEdges.iterator(); it.hasNext();) {
			NFAEdge edge = it.next();
			it.remove();
			disconnect(edge.m_Begin, edge);
		}
		/* ������г��� */
		for (Iterator<NFAEdge> it = status.m_OutEdges.iterator(); it.hasNext();) {
			NFAEdge edge = it.next();
			it.remove();
			disconnect(status, edge);
		}
		m_StatusPool.restore(status);
	}

	@Override
	public void visitBegin(Charset node) {
		enter();
		ENFA enfa = new ENFA();
		enfa.m_Begin = m_StatusPool.take();
		enfa.m_End = m_StatusPool.take();
		for (CharacterRange range : m_Map.getRanges()) {// ���������ַ�����
			if (node.include(range.m_chLowerBound)) {// ���ڵ�ǰ��㷶Χ�ڣ�����ӱ�
				NFAEdge edge = connect(enfa.m_Begin, enfa.m_End);// ��������״̬
				edge.m_Data.m_Action = EdgeType.CHARSET;// �ַ�����
				edge.m_Data.m_Param = m_Map.find(range.m_chLowerBound);
			}
		}
		storeENFA(enfa);
	}

	@Override
	public void visitBegin(Constructure node) {
		enter();
		enterChildren();
	}

	@Override
	public void visitBegin(Repetition node) {
		enter();
		enterChildren();
	}

	@Override
	public void visitEnd(Charset node) {
		leave();
	}

	@Override
	public void visitEnd(Constructure node) {
		leaveChildren();
		ENFA result = null;
		if (!node.m_bBranch) {
			/* ����ǰNFA������ͬÿ���ӽ������˴��� */
			for (ENFA enfa : m_childNFA) {
				if (result == null) {
					result = m_childNFA.get(0);
				} else {
					connect(result.m_End, enfa.m_Begin);
					result.m_End = enfa.m_End;
				}
			}
		} else {
			result = new ENFA();
			result.m_Begin = m_StatusPool.take();
			result.m_End = m_StatusPool.take();
			/* ����ǰNFA������ͬÿ���ӽ������˲��� */
			for (ENFA enfa : m_childNFA) {
				connect(result.m_Begin, enfa.m_Begin);
				connect(enfa.m_End, result.m_End);
			}
		}
		storeENFA(result);
		leave();
	}

	@Override
	public void visitEnd(Repetition node) {
		leaveChildren();
		/* ������ͼ���� */
		ArrayList<ENFA> subENFAList = new ArrayList<ENFA>();
		ENFA enfa = new ENFA();
		enfa.m_Begin = m_childNFA.get(0).m_Begin;
		enfa.m_End = m_childNFA.get(0).m_End;
		int count = Integer.max(node.m_iLowerBound, node.m_iUpperBound);
		subENFAList.add(enfa);
		/* ѭ������ENFA */
		for (int i = 1; i <= count; i++) {
			subENFAList.add(copyENFA(enfa));
		}
		enfa = new ENFA();
		/* ����ѭ����ʼ���� */
		if (node.m_iLowerBound > 0) {
			enfa.m_Begin = m_childNFA.get(0).m_Begin;
			enfa.m_End = m_childNFA.get(0).m_End;
			for (int i = 1; i < node.m_iLowerBound; i++) {
				connect(enfa.m_End, subENFAList.get(i).m_Begin);// ������β
				enfa.m_End = subENFAList.get(i).m_End;
			}
		}
		if (node.m_iUpperBound != -1) {// ����ѭ��������ѭ����������
			for (int i = node.m_iLowerBound; i < node.m_iUpperBound; i++) {
				if (enfa.m_End != null) {
					connect(enfa.m_End, subENFAList.get(i).m_Begin);// ������β
				} else {
					enfa = subENFAList.get(i);
				}
				connect(subENFAList.get(i).m_Begin,
						subENFAList.get(node.m_iUpperBound - 1).m_End);
			}
		} else {// ����ѭ��
			NFAStatus tailBegin, tailEnd;
			if (enfa.m_End == null) {
				tailBegin = enfa.m_Begin = m_StatusPool.take();
				tailEnd = enfa.m_End = m_StatusPool.take();
			} else {
				tailBegin = enfa.m_End;
				tailEnd = enfa.m_End = m_StatusPool.take();
			}
			/* ��������ѭ���Ľ������� */
			connect(tailBegin, subENFAList.get(node.m_iLowerBound).m_Begin);
			connect(subENFAList.get(node.m_iLowerBound).m_End, tailBegin);
			connect(tailBegin, tailEnd);
		}
		/* ����ѭ����ͷβ���� */
		NFAStatus begin = m_StatusPool.take();
		NFAStatus end = m_StatusPool.take();
		connect(begin, enfa.m_Begin);
		connect(enfa.m_End, end);
		enfa.m_Begin = begin;
		enfa.m_End = end;
		storeENFA(enfa);
		leave();
	}

	/**
	 * ����ջ��NFA
	 * 
	 * @return ��ǰջ��NFA
	 */
	private ArrayList<ENFA> currentNFA() {
		return m_stkNFA.peek();
	}

	/**
	 * �洢NFA
	 * 
	 * @param enfa
	 *            ENFA
	 */
	private void storeENFA(ENFA enfa) {
		currentNFA().add(enfa);
	}

	/**
	 * ������
	 */
	private void enter() {
		if (m_iLevel == 0) {// �ս���ý��ʱ���������ӽ��
			enterChildren();
		}
		m_iLevel++;
	}

	/**
	 * �뿪���
	 */
	private void leave() {
		m_iLevel--;
		if (m_iLevel == 0) {// �ս���ý��ʱ���������ӽ��
			leaveChildren();
			store();
		}
	}

	/**
	 * �����ӽ��
	 */
	private void enterChildren() {
		m_stkNFA.push(new ArrayList<ENFA>());// �½�ENFA��
		m_childNFA = null;
	}

	/**
	 * �뿪�ӽ��
	 */
	private void leaveChildren() {
		m_childNFA = m_stkNFA.pop();
	}

	/**
	 * �洢���
	 */
	private void store() {
		ENFA enfa = m_childNFA.get(0);
		enfa.m_End.m_Data.m_bFinal = true;
		m_mainNFA = enfa;
	}

	/**
	 * ����ENFA
	 * 
	 * @param enfa
	 *            ENFA
	 * @return ����
	 */
	private ENFA copyENFA(ENFA enfa) {
		ArrayList<NFAStatus> srcStatusList = new ArrayList<NFAStatus>();// ��̬��
		ArrayList<NFAStatus> dstStatusList = new ArrayList<NFAStatus>();// ��̬��
		srcStatusList.addAll(getNFAStatusClosure(
				new BreadthFirstSearch<NFAEdge, NFAStatus>(), enfa.m_Begin)); // ��ȡ״̬�հ�
		/* ����״̬ */
		for (NFAStatus status : srcStatusList) {
			NFAStatus newStatus = m_StatusPool.take();
			newStatus.m_Data = status.m_Data;
			dstStatusList.add(newStatus);
		}
		/* ���Ʊ� */
		for (int i = 0; i < srcStatusList.size(); i++) {
			NFAStatus status = srcStatusList.get(i);
			for (NFAEdge edge : status.m_OutEdges) {
				NFAEdge newEdge = connect(dstStatusList.get(i),
						dstStatusList.get(srcStatusList.indexOf(edge.m_End)));
				newEdge.m_Data = edge.m_Data;
			}
		}
		/* �ҵ�ʼ̬����̬ */
		ENFA result = new ENFA();
		result.m_Begin = dstStatusList.get(srcStatusList.indexOf(enfa.m_Begin));
		result.m_End = dstStatusList.get(srcStatusList.indexOf(enfa.m_End));
		return result;
	}
	
	/**
	 * ��ȡ�ַ�ӳ���
	 */
	public CharacterMap getCharacterMap() {
		return m_Map;
	}

	/**
	 * ��ȡNFA״̬�հ�
	 * 
	 * @param bfs
	 *            �����㷨
	 * @param status
	 *            ��̬
	 * @return ��̬�հ�
	 */
	protected static ArrayList<NFAStatus> getNFAStatusClosure(
			BreadthFirstSearch<NFAEdge, NFAStatus> bfs, NFAStatus status) {
		status.visit(bfs);
		return bfs.m_Path;
	}

	/**
	 * �ַ���������
	 */
	public String getStatusString() {
		return m_Map.toString();
	}

	/**
	 * NFA����
	 */
	public String getNFAString() {
		StringBuilder sb = new StringBuilder();
		ArrayList<NFAStatus> statusList = getNFAStatusClosure(
				new BreadthFirstSearch<NFAEdge, NFAStatus>(), m_mainNFA.m_Begin);// ��ȡ״̬�հ�
		/* ����NFA���� */
		for (int i = 0; i < statusList.size(); i++) {
			NFAStatus status = statusList.get(i);
			/* ״̬ */
			sb.append("״̬[" + i + "]" + (status.m_Data.m_bFinal ? "[����]" : "")
					+ System.getProperty("line.separator"));
			/* �� */
			for (NFAEdge edge : status.m_OutEdges) {
				sb.append("\t�� => [" + statusList.indexOf(edge.m_End) + "]"
						+ System.getProperty("line.separator"));// ָ���
				sb.append("\t\t���� => " + edge.m_Data.m_Action.getName());
				switch (edge.m_Data.m_Action)// ����
				{
				case CHARSET:
					sb.append("\t" + m_Map.getRanges().get(edge.m_Data.m_Param));// ����
					break;
				case EPSILON:
					break;
				default:
					break;
				}
				sb.append(System.getProperty("line.separator"));
			}
		}
		return sb.toString();
	}
}
