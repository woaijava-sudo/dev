package priv.bajdcc.lexer.algorithm;

/**
 * 检测/读取单词的算法接口
 * 
 * @author bajdcc
 */
public interface ITokenAlgorithm {

	/**
	 * 当前算法是否接受相应Visitor对象（即是否匹配）
	 * 
	 * @param visitor
	 *            字符串迭代对象
	 * @return 算法匹配结果
	 */
	boolean accept(ITokenVisitor visitor);
}
