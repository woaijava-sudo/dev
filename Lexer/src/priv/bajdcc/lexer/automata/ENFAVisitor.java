package priv.bajdcc.lexer.automata;

import java.util.ArrayList;
import java.util.Stack;

import priv.bajdcc.lexer.regex.CharacterMap;
import priv.bajdcc.lexer.regex.CharacterRange;
import priv.bajdcc.lexer.regex.Charset;
import priv.bajdcc.lexer.regex.Constructure;
import priv.bajdcc.lexer.regex.IRegexComponentVisitor;
import priv.bajdcc.lexer.regex.Repetition;

/**
 * EpsilonNFA�����㷨��AST->ENFA��
 * 
 * @author bajdcc
 *
 */
public class ENFAVisitor implements IRegexComponentVisitor {

	/**
	 * NFA
	 */
	private NFA m_NFA = new NFA();

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
	private ENFA m_mainNFA = null;

	/**
	 * Sigma״̬��
	 */
	private CharacterMap m_Map = null;

	public ENFAVisitor(CharacterMap map) {
		m_Map = map;
	}

	@Override
	public void visitBegin(Charset node) {
		enter();
		ENFA enfa = new ENFA();
		enfa.m_Begin = m_NFA.m_StatusPool.take();
		enfa.m_End = m_NFA.m_StatusPool.take();
		for (CharacterRange range : m_Map.getRanges()) {// ���������ַ�����
			if (node.include(range.m_chLowerBound)) {// ���ڵ�ǰ��㷶Χ�ڣ�����ӱ�
				NFAEdge edge = m_NFA.connect(enfa.m_Begin, enfa.m_End);// ��������״̬
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
					m_NFA.connect(result.m_End, enfa.m_Begin);
					result.m_End = enfa.m_End;
				}
			}
		} else {
			result = new ENFA();
			result.m_Begin = m_NFA.m_StatusPool.take();
			result.m_End = m_NFA.m_StatusPool.take();
			/* ����ǰNFA������ͬÿ���ӽ������˲��� */
			for (ENFA enfa : m_childNFA) {
				m_NFA.connect(result.m_Begin, enfa.m_Begin);
				m_NFA.connect(enfa.m_End, result.m_End);
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
				m_NFA.connect(enfa.m_End, subENFAList.get(i).m_Begin);// ������β
				enfa.m_End = subENFAList.get(i).m_End;
			}
		}
		if (node.m_iUpperBound != -1) {// ����ѭ��������ѭ����������
			for (int i = node.m_iLowerBound; i < node.m_iUpperBound; i++) {
				if (enfa.m_End != null) {
					m_NFA.connect(enfa.m_End, subENFAList.get(i).m_Begin);// ������β
				} else {
					enfa = subENFAList.get(i);
				}
				m_NFA.connect(subENFAList.get(i).m_Begin,
						subENFAList.get(node.m_iUpperBound - 1).m_End);
			}
		} else {// ����ѭ��
			NFAStatus tailBegin, tailEnd;
			if (enfa.m_End == null) {
				tailBegin = enfa.m_Begin = m_NFA.m_StatusPool.take();
				tailEnd = enfa.m_End = m_NFA.m_StatusPool.take();
			} else {
				tailBegin = enfa.m_End;
				tailEnd = enfa.m_End = m_NFA.m_StatusPool.take();
			}
			/* ��������ѭ���Ľ������� */
			m_NFA.connect(tailBegin,
					subENFAList.get(node.m_iLowerBound).m_Begin);
			m_NFA.connect(subENFAList.get(node.m_iLowerBound).m_End, tailBegin);
			m_NFA.connect(tailBegin, tailEnd);
		}
		/* ����ѭ����ͷβ���� */
		NFAStatus begin = m_NFA.m_StatusPool.take();
		NFAStatus end = m_NFA.m_StatusPool.take();
		m_NFA.connect(begin, enfa.m_Begin);
		m_NFA.connect(enfa.m_End, end);
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
		ArrayList<NFAStatus> srcStatusList = new ArrayList<NFAStatus>();// ��ʼ״̬��
		ArrayList<NFAStatus> dstStatusList = new ArrayList<NFAStatus>();// Ŀ��״̬��
		BreadthFirstSearch<NFAEdge, NFAStatus> bfs = new BreadthFirstSearch<NFAEdge, NFAStatus>() {
			@Override
			public boolean testEdge(NFAEdge edge) {
				return true;// �������б�
			}
		};
		enfa.m_Begin.visit(bfs);// ��ȡENFA������״̬
		srcStatusList.addAll(bfs.m_Path);
		/* ����״̬ */
		for (NFAStatus status : srcStatusList) {
			NFAStatus newStatus = m_NFA.m_StatusPool.take();
			newStatus.m_Data = status.m_Data;
			dstStatusList.add(newStatus);
		}
		/* ���Ʊ� */
		for (int i = 0; i < srcStatusList.size(); i++) {
			NFAStatus status = srcStatusList.get(i);
			for (NFAEdge edge : status.m_OutEdges) {
				NFAEdge newEdge = m_NFA.connect(dstStatusList.get(i),
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

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		BreadthFirstSearch<NFAEdge, NFAStatus> bfs = new BreadthFirstSearch<NFAEdge, NFAStatus>() {
			@Override
			public boolean testEdge(NFAEdge edge) {
				return true;// �������б�
			}
		};
		m_mainNFA.m_Begin.visit(bfs);// ��ȡENFA������״̬
		ArrayList<NFAStatus> statusList = bfs.m_Path;// ״̬��
		/* ����ENFA���� */
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
