#include<bits/stdc++.h>
using namespace std;

struct TreeNode{
    int val;
    TreeNode *left;
    TreeNode *right;
  TreeNode(int v)
  {
    val = v;
    left = NULL;
    right = NULL;
  }
};

class Solution {
public:

  vector<vector<int>> ans;
  int tmp_height;
  void dfs(TreeNode* s, int height)
  {
    if(s == NULL) // no node
      {
	return;
      }
    printf("dfs: node %d\n", s->val);
    if(height < tmp_height) // new height
    {
      vector<int> tmp_row;
      tmp_row.push_back(s->val);
      ans.push_back(tmp_row);
      tmp_height++;
    }
    printf("check 1\n");
    ans[height].push_back(s->val);
    printf("check 2\n");
    dfs(s->left, height+1);
    dfs(s->right, height+1);
  }
  vector<vector<int>> levelOrder(TreeNode* root)
  {
    tmp_height = -1;
    dfs(root, 0);    
    return ans;
  }
};

TreeNode make_tree()
{
  TreeNode root(1);
  TreeNode node1(2);
  TreeNode node2(3);
  TreeNode node3(4);
  root.left = &node1;
  root.right = &node2;
  node1.left = &node3;  
  return root;
}

int main()
{
  Solution obj;
  vector<vector<int>> ans;
  TreeNode root = make_tree();
  ans = obj.levelOrder(&root);
  for(int i=0; i<ans.size(); i++)
    {
      for(int j=0; j<ans[i].size(); j++)
	printf("%d ", ans[i][j]);
      printf("\n");
    }
  return 0;
}
