package priv.bajdcc.lexer.automata.dfa;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.ListIterator;

import org.vibur.objectpool.ConcurrentLinkedPool;
import org.vibur.objectpool.PoolService;

import priv.bajdcc.lexer.automata.BreadthFirstSearch;
import priv.bajdcc.lexer.automata.EdgeType;
import priv.bajdcc.lexer.automata.dfa.DFAStatus;
import priv.bajdcc.lexer.automata.nfa.NFA;
import priv.bajdcc.lexer.automata.nfa.NFAEdge;
import priv.bajdcc.lexer.automata.nfa.NFAStatus;
import priv.bajdcc.lexer.regex.IRegexComponent;
import priv.bajdcc.lexer.utility.ObjectFactory;

/**
 * ȷ�����Զ�����DFA��
 * 
 * @author bajdcc
 *
 */
public class DFA extends NFA {
	/**
	 * �߶����
	 */
	private PoolService<DFAEdge> m_EdgesPool = new ConcurrentLinkedPool<DFAEdge>(
			new ObjectFactory<DFAEdge>() {
				public DFAEdge create() {
					return new DFAEdge();
				};
			}, 1024, 10240, false);

	/**
	 * ״̬�����
	 */
	private PoolService<DFAStatus> m_StatusPool = new ConcurrentLinkedPool<DFAStatus>(
			new ObjectFactory<DFAStatus>() {
				public DFAStatus create() {
					return new DFAStatus();
				};
			}, 1024, 10240, false);

	/**
	 * DFA״̬����
	 */
	private DFAStatus m_mainDFA = null;

	public DFA(IRegexComponent exp, boolean debug) {
		super(exp, debug);
		transfer();
	}

	/**
	 * ת��
	 */
	private void transfer() {
		deleteEpsilonEdges();
		if (m_bDebug) {
			System.out.println("#### ����Epsilon�� ####");
			System.out.println(getNFAString());
		}
		determine();
		if (m_bDebug) {
			System.out.println("#### ȷ���� ####");
			System.out.println(getDFAString());
			System.out.println("#### ״̬ת�ƾ��� ####");
			System.out.println(getDFATableString());
		}
		minimization();
		if (m_bDebug) {
			System.out.println("#### ��С�� ####");
			System.out.println(getDFAString());
			System.out.println("#### ״̬ת�ƾ��� ####");
			System.out.println(getDFATableString());
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
	protected DFAEdge connect(DFAStatus begin, DFAStatus end) {
		DFAEdge edge = m_EdgesPool.take();// ����һ���±�
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
	protected void disconnect(DFAStatus status, DFAEdge edge) {
		edge.m_Begin.m_OutEdges.remove(edge);
		edge.m_End.m_InEdges.remove(edge);// ��ǰ�ߵĽ���״̬����߼���ȥ����ǰ��
		m_EdgesPool.restore(edge);
	}

	/**
	 * �Ͽ�ĳ��״̬�����б�
	 * 
	 * @param begin
	 *            ĳ״̬
	 */
	protected void disconnect(DFAStatus status) {
		/* ���������� */
		for (Iterator<DFAEdge> it = status.m_InEdges.iterator(); it.hasNext();) {
			DFAEdge edge = it.next();
			it.remove();
			disconnect(edge.m_Begin, edge);
		}
		/* ������г��� */
		for (Iterator<DFAEdge> it = status.m_OutEdges.iterator(); it.hasNext();) {
			DFAEdge edge = it.next();
			it.remove();
			disconnect(status, edge);
		}
		m_StatusPool.restore(status);
	}

	/**
	 * ��ȡDFA״̬�հ�
	 * 
	 * @param bfs
	 *            �����㷨
	 * @param status
	 *            ��̬
	 * @return ��̬�հ�
	 */
	protected static ArrayList<DFAStatus> getDFAStatusClosure(
			BreadthFirstSearch<DFAEdge, DFAStatus> bfs, DFAStatus status) {
		status.visit(bfs);
		return bfs.m_Path;
	}

	/**
	 * ���DFA״̬ת������
	 */
	public ArrayList<DFAStatus> getDFATable() {
		return getDFAStatusClosure(
				new BreadthFirstSearch<DFAEdge, DFAStatus>(), m_mainDFA);
	}

	/**
	 * ȥ��Epsilon��
	 */
	private void deleteEpsilonEdges() {
		ArrayList<NFAStatus> NFAStatusList = getNFAStatusClosure(
				new BreadthFirstSearch<NFAEdge, NFAStatus>(), m_mainNFA.m_Begin);// ��ȡ״̬�հ�
		ArrayList<NFAStatus> unaccessiableList = new ArrayList<NFAStatus>();// ���ɵ���״̬����
		for (NFAStatus status : NFAStatusList) {
			boolean epsilon = true;
			for (NFAEdge edge : status.m_InEdges) {
				if (edge.m_Data.m_Action != EdgeType.EPSILON) {// ����Epsilon��
					epsilon = false;// ��ǰ�ɵ���
					break;
				}
			}
			if (epsilon) {
				unaccessiableList.add(status);// ����������ΪEpsilon�ߣ��򲻿ɵ���
			}
		}
		unaccessiableList.remove(m_mainNFA.m_Begin);// ��̬��Ϊ��Ч
		BreadthFirstSearch<NFAEdge, NFAStatus> epsilonBFS = new BreadthFirstSearch<NFAEdge, NFAStatus>() {
			@Override
			public boolean testEdge(NFAEdge edge) {
				return edge.m_Data.m_Action == EdgeType.EPSILON;
			}
		};
		/* ����������Ч״̬ */
		for (NFAStatus status : NFAStatusList) {
			if (!unaccessiableList.contains(status)) {// ��Ϊ��Ч״̬
				/* ��ȡ��ǰ״̬��Epsilon�հ� */
				ArrayList<NFAStatus> epsilonClosure = getNFAStatusClosure(
						epsilonBFS, status);
				/* ȥ������״̬ */
				epsilonClosure.remove(status);
				/* ����Epsilon�հ���״̬ */
				for (NFAStatus epsilonStatus : epsilonClosure) {
					if (epsilonStatus.m_Data.m_bFinal) {
						/* ����հ�������̬����ǰ״̬Ϊ��̬ */
						status.m_Data.m_bFinal = true;
					}
					/* �����հ������б� */
					for (NFAEdge edge : epsilonStatus.m_OutEdges) {
						if (edge.m_Data.m_Action != EdgeType.EPSILON) {
							/* �����ǰ�߲���Epsilon�ߣ��ͽ��հ��е���Ч����ӵ���ǰ״̬ */
							connect(status, edge.m_End).m_Data = edge.m_Data;
						}
					}
				}
			}
		}
		/* ɾ��Epsilon�� */
		for (NFAStatus status : NFAStatusList) {
			for (Iterator<NFAEdge> it = status.m_OutEdges.iterator(); it
					.hasNext();) {
				NFAEdge edge = it.next();
				if (edge.m_Data.m_Action == EdgeType.EPSILON) {
					it.remove();
					disconnect(status, edge);// ɾ��Epsilon��
				}
			}
		}
		/* ɾ����Ч״̬ */
		for (NFAStatus status : unaccessiableList) {
			NFAStatusList.remove(status);// ɾ����Ч״̬
			disconnect(status);// ɾ����״̬�йص����б�
		}
		unaccessiableList.clear();
		/* ɾ���ظ��� */
		for (NFAStatus status : NFAStatusList) {
			for (int i = 0; i < status.m_OutEdges.size() - 1; i++) {
				NFAEdge edge1 = status.m_OutEdges.get(i);
				for (ListIterator<NFAEdge> it2 = status.m_OutEdges
						.listIterator(i + 1); it2.hasNext();) {
					NFAEdge edge2 = it2.next();
					if (edge1.m_End == edge2.m_End
							&& edge1.m_Data.m_Action == edge2.m_Data.m_Action
							&& edge1.m_Data.m_Param == edge2.m_Data.m_Param) {
						it2.remove();
						disconnect(status, edge2);
					}
				}
			}
		}
	}

	/**
	 * NFAȷ������תΪDFA
	 */
	private void determine() {
		/* ȡ��NFA����״̬ */
		ArrayList<NFAStatus> NFAStatusList = getNFAStatusClosure(
				new BreadthFirstSearch<NFAEdge, NFAStatus>(), m_mainNFA.m_Begin);
		ArrayList<DFAStatus> DFAStatusList = new ArrayList<DFAStatus>();
		/* ��ϣ����������DFA״̬��Ĳ��� */
		HashMap<String, Integer> DFAStatusListMap = new HashMap<String, Integer>();
		DFAStatus initStatus = m_StatusPool.take();
		initStatus.m_Data.m_bFinal = m_mainNFA.m_Begin.m_Data.m_bFinal;// �Ƿ���̬
		initStatus.m_Data.m_NFAStatus.add(m_mainNFA.m_Begin);// DFA[0]=NFA��̬����
		DFAStatusList.add(initStatus);
		DFAStatusListMap.put(initStatus.m_Data.getStatusString(NFAStatusList),
				0);
		/* ����DFA�� */
		for (int i = 0; i < DFAStatusList.size(); i++) {
			DFAStatus dfaStatus = DFAStatusList.get(i);
			ArrayList<DFAEdgeBag> bags = new ArrayList<DFAEdgeBag>();
			/* ������ǰNFA״̬���ϵ����б� */
			for (NFAStatus nfaStatus : dfaStatus.m_Data.m_NFAStatus) {
				for (NFAEdge nfaEdge : nfaStatus.m_OutEdges) {
					DFAEdgeBag dfaBag = null;
					for (DFAEdgeBag bag : bags) {
						/* ����Ƿ��ڱ��� */
						if (nfaEdge.m_Data.m_Action == bag.m_Action
								&& nfaEdge.m_Data.m_Param == bag.m_Param) {
							dfaBag = bag;
							break;
						}
					}
					/* �������ڣ����½� */
					if (dfaBag == null) {
						dfaBag = new DFAEdgeBag();
						dfaBag.m_Action = nfaEdge.m_Data.m_Action;
						dfaBag.m_Param = nfaEdge.m_Data.m_Param;
						bags.add(dfaBag);
					}
					/* ��ӵ�ǰ�� */
					dfaBag.m_NFAEdges.add(nfaEdge);
					/* ��ӵ�ǰ״̬ */
					dfaBag.m_NFAStatus.add(nfaEdge.m_End);
				}
			}
			/* ������ǰ������DFA�� */
			for (DFAEdgeBag bag : bags) {
				/* ���DFAָ���״̬�Ƿ���� */
				DFAStatus status = null;
				/* ��ϣ�ַ��� */
				String hash = bag.getStatusString(NFAStatusList);
				if (DFAStatusListMap.containsKey(bag
						.getStatusString(NFAStatusList))) {
					status = DFAStatusList.get(DFAStatusListMap.get(hash));
				} else {// ������DFA
					status = m_StatusPool.take();
					status.m_Data.m_NFAStatus = new ArrayList<NFAStatus>(
							bag.m_NFAStatus);
					/* �����̬ */
					for (NFAStatus nfaStatus : status.m_Data.m_NFAStatus) {
						if (nfaStatus.m_Data.m_bFinal) {
							status.m_Data.m_bFinal = true;
							break;
						}
					}
					DFAStatusList.add(status);
					DFAStatusListMap.put(hash, DFAStatusList.size() - 1);
				}
				/* ����DFA�� */
				DFAEdge edge = connect(dfaStatus, status);
				edge.m_Data.m_Action = bag.m_Action;
				edge.m_Data.m_Param = bag.m_Param;
				edge.m_Data.m_NFAEdges = bag.m_NFAEdges;
			}
		}
		m_mainDFA = DFAStatusList.get(0);
	}

	/**
	 * DFA��С��
	 */
	private void minimization() {
		/* ��̬���� */
		ArrayList<Integer> finalStatus = new ArrayList<Integer>();
		/* ����̬���� */
		ArrayList<Integer> nonFinalStatus = new ArrayList<Integer>();
		/* DFA״̬ת�Ʊ������̬���� */
		int[][] transition = buildTransition(finalStatus);
		/* ������̬���Ϻ�״̬���ϵĹ�ϣ�� */
		for (int i = 0; i < transition.length; i++) {
			if (!finalStatus.contains(i)) {
				nonFinalStatus.add(i);// ��ӷ���̬���
			}
		}
		/* DFA״̬�� */
		ArrayList<DFAStatus> statusList = getDFATable();
		/* ������̬ */
		mergeStatus(partition(finalStatus, transition), statusList);
		/* �������̬ */
		mergeStatus(partition(nonFinalStatus, transition), statusList);
	}

	/**
	 * ��С������
	 * 
	 * @param statusList
	 *            ��ʼ����
	 * @param transition
	 *            ״̬ת�Ʊ�
	 * @return ����
	 */
	private ArrayList<ArrayList<Integer>> partition(
			ArrayList<Integer> statusList, int[][] transition) {
		if (statusList.size() == 1) {
			/* ��Ž�� */
			ArrayList<ArrayList<Integer>> pat = new ArrayList<ArrayList<Integer>>();
			pat.add(new ArrayList<Integer>(statusList.get(0)));
			return pat;
		} else {
			/* ���ڲ�����ͬ״̬ */
			HashMap<String, ArrayList<Integer>> map = new HashMap<String, ArrayList<Integer>>();
			for (int status : statusList) {
				/* ���״̬hash */
				String hash = getStatusLineString(transition[status]);
				/* ״̬�Ƿ���ֹ� */
				if (map.containsKey(hash)) {
					/* ״̬�ظ��������ϸ���ͬ״̬�ļ��� */
					map.get(hash).add(status);
				} else {
					/* ǰ�γ��֣��������鱣���� */
					ArrayList<Integer> set = new ArrayList<Integer>();
					set.add(status);
					map.put(hash, set);
				}
			}
			return new ArrayList<ArrayList<Integer>>(map.values());
		}
	}

	/**
	 * �ϲ���ͬ״̬
	 * 
	 * @param pat
	 *            ״̬����
	 * @param statusList
	 *            ״̬ת�Ʊ�
	 */
	private void mergeStatus(ArrayList<ArrayList<Integer>> pat,
			ArrayList<DFAStatus> statusList) {
		/* ����Ҫ����Ķ�״̬�ϲ��Ļ��� */
		ArrayList<ArrayList<Integer>> dealWith = new ArrayList<ArrayList<Integer>>();
		for (ArrayList<Integer> collection : pat) {
			if (collection.size() > 1) {// �ж��״̬
				dealWith.add(collection);
			}
		}
		/* �ϲ�ÿһ���� */
		for (ArrayList<Integer> collection : dealWith) {
			/* Ŀ��״̬Ϊ�����е�һ��״̬������״̬���ϲ� */
			int dstStatus = collection.get(0);
			/* Ŀ��״̬ */
			DFAStatus status = statusList.get(dstStatus);
			for (int i = 1; i < collection.size(); i++) {
				/* �ظ���״̬ */
				int srcStatus = collection.get(i);
				DFAStatus dupStatus = statusList.get(srcStatus);
				/* �����ظ�״̬����� */
				ArrayList<DFAEdge> edges = new ArrayList<DFAEdge>(
						dupStatus.m_InEdges);
				/* ��ָ���ظ�״̬�ı߸�Ϊָ��Ŀ��״̬�ı� */
				for (DFAEdge edge : edges) {
					/* ���Ʊ� */
					connect(edge.m_Begin, status).m_Data = edge.m_Data;
				}
				/* ȥ���ظ�״̬ */
				disconnect(dupStatus);
			}
		}
	}

	/**
	 * ��ȡDFA״̬ת�Ʊ�ĳ�е��ַ���
	 * 
	 * @param line
	 *            ĳ�е���������
	 * @return ��ϣ�ַ���
	 */
	private String getStatusLineString(int[] line) {
		StringBuilder sb = new StringBuilder();
		for (int i : line) {
			sb.append(i + ",");
		}
		return sb.toString();
	}

	/**
	 * ����״̬
	 * 
	 * @param finalStatus
	 * @return ״̬ת������
	 */
	public int[][] buildTransition(Collection<Integer> finalStatus) {
		finalStatus.clear();
		/* DFA״̬�� */
		ArrayList<DFAStatus> statusList = getDFATable();
		/* ����״̬ת�ƾ��� */
		int[][] transition = new int[statusList.size()][m_Map.getRanges()
				.size()];
		/* ���״̬ת�Ʊ� */
		for (int i = 0; i < statusList.size(); i++) {
			DFAStatus status = statusList.get(i);
			if (status.m_Data.m_bFinal) {
				finalStatus.add(i);// �����̬
			}
			for (int j = 0; j < transition[i].length; j++) {
				transition[i][j] = -1;// ����Ч���-1
			}
			for (DFAEdge edge : status.m_OutEdges) {
				if (edge.m_Data.m_Action == EdgeType.CHARSET) {
					transition[i][edge.m_Data.m_Param] = statusList
							.indexOf(edge.m_End);
				}
			}
		}
		return transition;
	}

	/**
	 * �ṩDFA����
	 */
	private String getDFAString() {
		/* ȡ��NFA����״̬ */
		ArrayList<NFAStatus> NFAStatusList = getNFAStatusClosure(
				new BreadthFirstSearch<NFAEdge, NFAStatus>(), m_mainNFA.m_Begin);
		/* ȡ��DFA����״̬ */
		ArrayList<DFAStatus> DFAStatusList = getDFAStatusClosure(
				new BreadthFirstSearch<DFAEdge, DFAStatus>(), m_mainDFA);
		StringBuilder sb = new StringBuilder();
		/* ����DFA���� */
		for (int i = 0; i < DFAStatusList.size(); i++) {
			DFAStatus status = DFAStatusList.get(i);
			/* ״̬ */
			sb.append("״̬[" + i + "]" + (status.m_Data.m_bFinal ? "[����]" : "")
					+ " => " + status.m_Data.getStatusString(NFAStatusList)
					+ System.getProperty("line.separator"));
			/* �� */
			for (DFAEdge edge : status.m_OutEdges) {
				sb.append("\t�� => [" + DFAStatusList.indexOf(edge.m_End) + "]"
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
	
	/**
	 * ��ȡ״̬ת�ƾ�������
	 */
	public String getDFATableString() {
		int[][] transition = buildTransition(new ArrayList<Integer>());
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < transition.length; i++) {
			for (int j = 0; j < transition[i].length; j++) {
				sb.append("\t" + transition[i][j]);
			}
			sb.append(System.getProperty("line.separator"));
		}
		return sb.toString();
	}
}