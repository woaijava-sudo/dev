package cn.njnu.edu.bajdcc;

import java.util.ArrayList;

/**
 * @author bajdcc ��ȡ�����㷨���ϣ��������֡��ַ����ȣ�
 */
public class TokenAlgoritemCollection {
	private ArrayList<ITokenAlgorithm> arrAlgorithms = new ArrayList<ITokenAlgorithm>();

	public void attach(ITokenAlgorithm alg) {
		arrAlgorithms.add(alg);
	}

	public void detach(ITokenAlgorithm alg) {
		arrAlgorithms.remove(alg);
	}

	public void scan(ITokenVisitor visitor) {
		for (ITokenAlgorithm alg : arrAlgorithms) {
			alg.accept(visitor);
		}
	}
}
