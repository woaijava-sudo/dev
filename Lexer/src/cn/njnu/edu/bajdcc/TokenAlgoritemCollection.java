package cn.njnu.edu.bajdcc;

import java.util.ArrayList;

/**
 * @author bajdcc 抽取单词算法集合（包含数字、字符串等）
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
