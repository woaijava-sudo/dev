package priv.bajdcc.lexer.algorithm;

import java.util.ArrayList;

/**
 * ���ڳ�ȡ���ʵ��㷨���ϣ��������֡��ַ����ȣ�
 * 
 * @author bajdcc
 */
public class TokenAlgorithmCollection {
	private ArrayList<ITokenAlgorithm> m_arrAlgorithms = new ArrayList<ITokenAlgorithm>();

	public void attach(ITokenAlgorithm alg) {
		m_arrAlgorithms.add(alg);
	}

	public void detach(ITokenAlgorithm alg) {
		m_arrAlgorithms.remove(alg);
	}

	public boolean scan(ITokenVisitor visitor) {
		for (ITokenAlgorithm alg : m_arrAlgorithms) {
			if (alg.accept(visitor))
				return true;
		}
		return false;
	}
}
