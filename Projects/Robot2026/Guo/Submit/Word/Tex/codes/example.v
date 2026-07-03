// ============================================================
//  数码管动态扫描模块（Verilog 示例）
//  平台：Xilinx Artix-7, 100 MHz 时钟
// ============================================================
module seg_scan (
    input  wire        clk,
    input  wire        rst_n,
    input  wire [15:0] data,
    output reg  [3:0]  seg_sel,
    output reg  [7:0]  seg_code
);

    reg [15:0] cnt;
    reg [1:0]  scan_idx;

    // 分频产生约 1 kHz 扫描时钟
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            cnt <= 16'd0;
        else if (cnt == 16'd49_999)
            cnt <= 16'd0;
        else
            cnt <= cnt + 1'b1;
    end

    wire scan_clk = (cnt == 16'd49_999);

    // 扫描状态机
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            scan_idx <= 2'd0;
        else if (scan_clk)
            scan_idx <= scan_idx + 1'b1;
    end

    // 位选与段码输出（略）
    always @(*) begin
        seg_sel  = 4'b1111;
        seg_code = 8'h00;
        case (scan_idx)
            2'd0: seg_sel = 4'b1110;
            2'd1: seg_sel = 4'b1101;
            2'd2: seg_sel = 4'b1011;
            2'd3: seg_sel = 4'b0111;
        endcase
    end

endmodule
