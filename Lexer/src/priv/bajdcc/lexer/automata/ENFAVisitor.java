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
 * EpsilonNFA构成算法（AST->ENFA）
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
	 * 深度
	 */
	private int m_iLevel = 0;

	/**
	 * NFA栈
	 */
	private Stack<ArrayList<ENFA>> m_stkNFA = new Stack<ArrayList<ENFA>>();

	/**
	 * NFA子表
	 */
	private ArrayList<ENFA> m_childNFA = new ArrayList<ENFA>();

	/**
	 * ENFA
	 */
	private ENFA m_mainNFA = null;

	/**
	 * Sigma状态集
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
		for (CharacterRange range : m_Map.getRanges()) {// 遍历所有字符区间
			if (node.include(range.m_chLowerBound)) {// 若在当前结点范围内，则添加边
				NFAEdge edge = m_NFA.connect(enfa.m_Begin, enfa.m_End);// 连接两个状态
				edge.m_Data.m_Action = EdgeType.CHARSET;// 字符类型
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
			/* 将当前NFA的两端同每个子结点的两端串联 */
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
			/* 将当前NFA的两端同每个子结点的两端并联 */
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
		/* 构造子图副本 */
		ArrayList<ENFA> subENFAList = new ArrayList<ENFA>();
		ENFA enfa = new ENFA();
		enfa.m_Begin = m_childNFA.get(0).m_Begin;
		enfa.m_End = m_childNFA.get(0).m_End;
		int count = Integer.max(node.m_iLowerBound, node.m_iUpperBound);
		subENFAList.add(enfa);
		/* 循环复制ENFA */
		for (int i = 1; i <= count; i++) {
			subENFAList.add(copyENFA(enfa));
		}
		enfa = new ENFA();
		/* 构造循环起始部分 */
		if (node.m_iLowerBound > 0) {
			enfa.m_Begin = m_childNFA.get(0).m_Begin;
			enfa.m_End = m_childNFA.get(0).m_End;
			for (int i = 1; i < node.m_iLowerBound; i++) {
				m_NFA.connect(enfa.m_End, subENFAList.get(i).m_Begin);// 连接首尾
				enfa.m_End = subENFAList.get(i).m_End;
			}
		}
		if (node.m_iUpperBound != -1) {// 有限循环，构造循环结束部分
			for (int i = node.m_iLowerBound; i < node.m_iUpperBound; i++) {
				if (enfa.m_End != null) {
					m_NFA.connect(enfa.m_End, subENFAList.get(i).m_Begin);// 连接首尾
				} else {
					enfa = subENFAList.get(i);
				}
				m_NFA.connect(subENFAList.get(i).m_Begin,
						subENFAList.get(node.m_iUpperBound - 1).m_End);
			}
		} else {// 无限循环
			NFAStatus tailBegin, tailEnd;
			if (enfa.m_End == null) {
				tailBegin = enfa.m_Begin = m_NFA.m_StatusPool.take();
				tailEnd = enfa.m_End = m_NFA.m_StatusPool.take();
			} else {
				tailBegin = enfa.m_End;
				tailEnd = enfa.m_End = m_NFA.m_StatusPool.take();
			}
			/* 构造无限循环的结束部分 */
			m_NFA.connect(tailBegin,
					subENFAList.get(node.m_iLowerBound).m_Begin);
			m_NFA.connect(subENFAList.get(node.m_iLowerBound).m_End, tailBegin);
			m_NFA.connect(tailBegin, tailEnd);
		}
		/* 构造循环的头尾部分 */
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
	 * 返回栈顶NFA
	 * 
	 * @return 当前栈顶NFA
	 */
	private ArrayList<ENFA> currentNFA() {
		return m_stkNFA.peek();
	}

	/**
	 * 存储NFA
	 * 
	 * @param enfa
	 *            ENFA
	 */
	private void storeENFA(ENFA enfa) {
		currentNFA().add(enfa);
	}

	/**
	 * 进入结点
	 */
	private void enter() {
		if (m_iLevel == 0) {// 刚进入该结点时，访问其子结点
			enterChildren();
		}
		m_iLevel++;
	}

	/**
	 * 离开结点
	 */
	private void leave() {
		m_iLevel--;
		if (m_iLevel == 0) {// 刚进入该结点时，访问其子结点
			leaveChildren();
			store();
		}
	}

	/**
	 * 进入子结点
	 */
	private void enterChildren() {
		m_stkNFA.push(new ArrayList<ENFA>());// 新建ENFA表
		m_childNFA = null;
	}

	/**
	 * 离开子结点
	 */
	private void leaveChildren() {
		m_childNFA = m_stkNFA.pop();
	}

	/**
	 * 存储结果
	 */
	private void store() {
		ENFA enfa = m_childNFA.get(0);
		enfa.m_End.m_Data.m_bFinal = true;
		m_mainNFA = enfa;
	}

	/**
	 * 复制ENFA
	 * 
	 * @param enfa
	 *            ENFA
	 * @return 副本
	 */
	private ENFA copyENFA(ENFA enfa) {
		ArrayList<NFAStatus> srcStatusList = new ArrayList<NFAStatus>();// 起始状态表
		ArrayList<NFAStatus> dstStatusList = new ArrayList<NFAStatus>();// 目标状态表
		BreadthFirstSearch<NFAEdge, NFAStatus> bfs = new BreadthFirstSearch<NFAEdge, NFAStatus>() {
			@Override
			public boolean testEdge(NFAEdge edge) {
				return true;// 测试所有边
			}
		};
		enfa.m_Begin.visit(bfs);// 获取ENFA的所有状态
		srcStatusList.addAll(bfs.m_Path);
		/* 复制状态 */
		for (NFAStatus status : srcStatusList) {
			NFAStatus newStatus = m_NFA.m_StatusPool.take();
			newStatus.m_Data = status.m_Data;
			dstStatusList.add(newStatus);
		}
		/* 复制边 */
		for (int i = 0; i < srcStatusList.size(); i++) {
			NFAStatus status = srcStatusList.get(i);
			for (NFAEdge edge : status.m_OutEdges) {
				NFAEdge newEdge = m_NFA.connect(dstStatusList.get(i),
						dstStatusList.get(srcStatusList.indexOf(edge.m_End)));
				newEdge.m_Data = edge.m_Data;
			}
		}
		/* 找到始态和终态 */
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
				return true;// 测试所有边
			}
		};
		m_mainNFA.m_Begin.visit(bfs);// 获取ENFA的所有状态
		ArrayList<NFAStatus> statusList = bfs.m_Path;// 状态表
		/* 生成ENFA描述 */
		for (int i = 0; i < statusList.size(); i++) {
			NFAStatus status = statusList.get(i);
			/* 状态 */
			sb.append("状态[" + i + "]" + (status.m_Data.m_bFinal ? "[结束]" : "")
					+ System.getProperty("line.separator"));
			/* 边 */
			for (NFAEdge edge : status.m_OutEdges) {
				sb.append("\t边 => [" + statusList.indexOf(edge.m_End) + "]"
						+ System.getProperty("line.separator"));// 指向边
				sb.append("\t\t类型 => " + edge.m_Data.m_Action.getName());
				switch (edge.m_Data.m_Action)// 类型
				{
				case CHARSET:
					sb.append("\t" + m_Map.getRanges().get(edge.m_Data.m_Param));// 区间
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
