package cn.njnu.edu.bajdcc.lexer.algorithm;

import java.util.ArrayList;

/**
 * ���ڳ�ȡ���ʵ��㷨���ϣ��������֡��ַ����ȣ�
 * 
 * @author bajdcc
 */
public class TokenAlgorithmCollection {
	private ArrayList<ITokenAlgorithm> arrAlgorithms = new ArrayList<ITokenAlgorithm>();

	public void attach(ITokenAlgorithm alg) {
		arrAlgorithms.add(alg);
	}

	public void detach(ITokenAlgorithm alg) {
		arrAlgorithms.remove(alg);
	}

	public boolean scan(ITokenVisitor visitor) {
		for (ITokenAlgorithm alg : arrAlgorithms) {
			if (alg.accept(visitor))
				return true;
		}
		return false;
	}
}
