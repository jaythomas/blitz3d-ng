module Blitz3D
  module AST
    class AssNode < Node
      attr_accessor :var, :expr

      def initialize(json)
        @var = Node.load json['var']
        @expr = Node.load json['expr']
      end

      def to_c
        "#{var.to_c} = #{expr.to_c}"
      end
    end
  end
end