package cn.njnu.edu.bajdcc;

/**
 * @author bajdcc
 * 检测/读取单词的算法接口
 */
public interface ITokenAlgorithm {
	void accept(ITokenVisitor visitor);	
}
