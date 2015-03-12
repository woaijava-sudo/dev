package cn.njnu.edu.bajdcc.lexer.algorithm;

import java.util.ArrayList;

/**
 * 用于抽取单词的算法集合（包含数字、字符串等）
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
